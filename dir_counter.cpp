#include "dir_counter.h"

#include <stdio.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <map>
#include <utility>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <typeinfo>
#include <vector>

#include "err.h"
#include "counter.h"
#include "log.h"
#include "reader.h"

enum class RetCode {
    SUCCESS = 0x00,
    SSFI = 0x01,
    GENERIC = 0x02
};

Dir_Counter::Dir_Counter(const Dir_Counter& dc) :
        _dir_path(dc._dir_path), _files(dc._files), _fin(dc._fin), _mx(dc._mx), _workers(
                dc._workers) {
}

Dir_Counter::Dir_Counter(const int& workers, std::string dir_path) :
        _workers(workers), _dir_path(dir_path) {
    _mx = new std::mutex();
}

Dir_Counter::~Dir_Counter() {
    delete _mx;
}

void Dir_Counter::run() {
    log(LOC, "worker threads: %d", _workers);
    log(LOC, "target directory: %s", _dir_path.c_str());

    std::list<Counter*> counters;
    try {
        for (int i = 1; i <= _workers; i++) {
            log(LOC, "launching worker thread %d", i);
            counters.push_back(new Counter(i, this));
        }

        log(LOC, "launching filer thread");
        std::thread tfiler = std::thread([this] {this->filer();});

        log(LOC, "joining filer thread");
        tfiler.join();

        std::unique_lock<std::mutex> lck { *_mx };
        try {
            log(LOC, "signaling completion");
            _fin = true;
            lck.unlock();
        } catch (const std::exception& e) {
            lck.unlock();
            throw e;
        }

        int iter_no = 1;
        for (auto iter = counters.begin(); iter != counters.end(); iter++) {
            log(LOC, "joining worker thread %d", iter_no++);
            (*iter)->join();
        }

        while (!counters.empty()) {
            delete counters.front();
            counters.pop_front();
        }
    } catch (const std::exception& e) {
        while (!counters.empty()) {
            delete counters.front();
            counters.pop_front();
        }
        throw e;
    }
}

bool Dir_Counter::pop_file(std::string* file) {
    bool fin = false;
    std::unique_lock<std::mutex> lck { *_mx };
    try {
        if (!_files.empty()) {
            file->assign(_files.front());
            log(LOC, "popped %s", file->c_str());
            _files.pop_front();
        }
        fin = _fin;
        lck.unlock();
        return fin;
    } catch (const std::exception& e) {
        lck.unlock();
        throw e;
    }
}

void Dir_Counter::filer() {
    filer(_dir_path);
}

void Dir_Counter::filer(std::string path) {
    DIR *dir = nullptr;

    // Slow this down so there's some interleaving with the processing.
    usleep(100 * 10); // TODO delete

    try {

        struct stat file_info;
        if (stat(path.c_str(), &file_info) == -1) {
            throw SSFI_Ex(LOC, (const char*) nullptr, "Error stat-ing \"%s\"",
                    path.c_str());
        }

        if (S_ISDIR(file_info.st_mode)) {

            log(LOC, "reading directory %s", path.c_str());

            dir = opendir(path.c_str());
            if (dir == nullptr) {
                throw SSFI_Ex(LOC, (const char*) nullptr,
                        "Error opening directory \"%s\"", path.c_str());
            }

            struct dirent *de;
            while ((de = readdir(dir)) != nullptr) {

                std::string name = std::string(de->d_name);
                if ((name.compare(".") == 0) || (name.compare("..") == 0)) {
                    continue;
                }

                std::string sub_path = std::string(path);
                sub_path.append("/");
                sub_path.append(de->d_name);

                log(LOC, "child path: %s", sub_path.c_str());

                filer(sub_path);
            }

        } else if ((path.length() >= _suffix.length())
                && (path.compare(path.length() - _suffix.length(),
                        _suffix.length(), _suffix) == 0)) {
            log(LOC, "found .txt file: %s", path.c_str());

            std::unique_lock<std::mutex> lck {*_mx};
            try {
                log(LOC, "pushing file: %s", path.c_str());
                _files.push_back(move(path));
                lck.unlock();
            } catch (const std::exception& e) {
                lck.unlock();
                throw e;
            }
        } else {
            log(LOC, "skipping non-txt file: %s", path.c_str());
        }

        closedir(dir);
    } catch (const std::exception& e) {
        if (dir != nullptr) {
            closedir(dir);
        }
        throw e;
    }

}

