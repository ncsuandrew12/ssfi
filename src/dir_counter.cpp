#include "dir_counter.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
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

Dir_Counter::Dir_Counter(const Dir_Counter& dc) :
        _dir_path(dc._dir_path), _err(dc._err), _files(dc._files), _fin(
                dc._fin), _mx(dc._mx), _workers(dc._workers) {
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
    SSFI_Ex* ex = nullptr;
    try {
        log(LOC, "launching filer thread");
        std::thread tfiler = std::thread([this] {this->filer();});

        for (int i = 1; i <= _workers; i++) {
            log(LOC, "launching worker thread %d", i);
            counters.push_back(new Counter(i, this));
        }

        log(LOC, "joining filer thread");
        tfiler.join();

        if (_err != nullptr) {
            ex = _err;
        }

        log(LOC, "signaling completion");
        std::unique_lock <std::mutex> lck { *_mx };
        _fin = true;
        lck.unlock();

        std::map<std::string, long> words;

        int iter_no = 1;
        for (auto iter = counters.begin(); iter != counters.end(); iter++) {
            log(LOC, "joining worker thread %d", iter_no++);
            (*iter)->join();

            for (auto wi = (*iter)->_words.begin(); wi != (*iter)->_words.end();
                    wi++) {
                log(LOC, "word \"%s\": %d+=%d instances", wi->first.c_str(),
                        words[wi->first.c_str()], wi->second);
                words[wi->first.c_str()] += wi->second;
            }

            if ((*iter)->_err != nullptr) {
                if (ex == nullptr) {
                    ex = (*iter)->_err;
                } else {
                    delete (*iter)->_err;
                }
            }
        }

        if (ex != nullptr) {
            throw ex;
        }

        std::vector<std::string> most_common;
        const int mcl = 10;
        for (auto wi = words.begin(); wi != words.end(); wi++) {
            log(LOC, "word \"%s\": %d instances", wi->first.c_str(), wi->second);
            bool inserted = false;
            for (auto mci = most_common.begin(); mci != most_common.end(); mci++) {
                if (wi->second > words[*mci]) {
                    inserted = true;
                    most_common.insert(mci, wi->first.c_str());
                    break;
                }
            }
            if (!inserted && most_common.size() < mcl) {
                most_common.push_back(wi->first.c_str());
            }
            while (most_common.size() > mcl) {
                most_common.pop_back();
            }
        }
        for (auto mci = most_common.begin(); mci != most_common.end(); mci++) {
            log(LOC, "word \"%s\": %d instances", (*mci).c_str(), words[*mci]);
            printf("%s %d\n", (*mci).c_str(), words[*mci]);
        }

        while (!counters.empty()) {
            delete counters.front();
            counters.pop_front();
        }
    } catch (const std::exception& e) {
        if (ex != nullptr) {
            delete ex;
        }
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
    try {
        filer(_dir_path);
    } catch (const SSFI_Ex& e) {
        _err = new SSFI_Ex(LOC, (std::exception*) nullptr, e.what(), "%s",
                e.summary().c_str());
    } catch (const std::exception& e) {
        _err = new SSFI_Ex(LOC, (std::exception*) nullptr, e.what(), "%s",
                e.what());
    }
}

void Dir_Counter::filer(std::string path) {
    DIR *dir = nullptr;

    // Slow this down so there's some interleaving with the processing.
//    usleep(100 * 10); // TODO delete

    try {

        struct stat file_info;
        if (stat(path.c_str(), &file_info) == -1) {
            if (errno == ENOENT) {
                throw SSFI_Ex(LOC, (const char*) nullptr,
                        "Path does not exist: \"%s\"", path.c_str());
            }
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
    } catch (const SSFI_Ex& e) {
        if (dir != nullptr) {
            closedir(dir);
        }
        throw e;
    } catch (const std::exception& e) {
        if (dir != nullptr) {
            closedir(dir);
        }
        throw e;
    }

}

