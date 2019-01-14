#include <stdio.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <typeinfo>
#include <vector>

#include "err.h"
#include "log.h"

enum class RetCode {
    SUCCESS = 0x00,
    SSFI = 0x01,
    GENERIC = 0x02
};

void filer(std::string dir_path);

void worker(const int& id);

std::list<std::string> files;
bool fin = false;
std::mutex mx;
std::string suffix = std::string(".txt");

int main(int argc, char **argv) {
    RetCode ret = RetCode::SUCCESS;

    try {
        int worker_threads = 1;
        std::string dir_arg;

        for (int i = 1; i < argc; i++) {
            if (std::string(argv[i]) == "-t") {
                if (i >= argc - 1) {
                    throw SSFI_Ex(LOC,
                            new std::invalid_argument("no value provided"),
                            (const char*) nullptr,
                            "No value provided for \"%s\"", argv[i]);
                }
                try {
                    worker_threads = std::stoi(argv[i + 1]);
                    i++; // Skip over value to next option.
                } catch (std::invalid_argument e) {
                    throw SSFI_Ex(LOC, new std::invalid_argument(e),
                            (const char*) nullptr,
                            "%s: %s: Error parsing value for option \"%s\": \"%s\". Value must be number.",
                            typeid(e).name(), e.what(), argv[i], argv[i + 1]);
                }
            } else if (dir_arg.empty()) {
                dir_arg = std::string(argv[i]);
            } else {
                log(LOC, "Ignoring extraneous arg #%d: %s", i, argv[i]);
            }
        }

        if (dir_arg.empty()) {
            throw SSFI_Ex(LOC,
                    new std::invalid_argument("No target directory provided"),
                    (const char*) nullptr, (const char*) nullptr);
        }

        log(LOC, "worker threads: %d", worker_threads);
        log(LOC, "target directory: %s", dir_arg.c_str());

        std::list<std::thread> workers;
        for (int i = 1; i <= worker_threads; i++) {
            log(LOC, "launching worker thread %d", i);
            std::thread tworker {worker, i};
            workers.push_back(move(tworker));
        }

        log(LOC, "launching filer thread");
        std::thread tfiler {filer, dir_arg};

        log(LOC, "joining filer thread");
        tfiler.join();
        log(LOC, "joined filer thread");

        std::unique_lock<std::mutex> lck {mx};
        log(LOC, "signaling completion");
        fin = true;
        lck.unlock();

        int iter_no = 1;
        for (auto iter = workers.begin(); iter != workers.end(); iter++ ) {
            log(LOC, "joining worker thread %d", iter_no++);
            (*iter).join();
        }

        log(LOC, "Done.");

    } catch (const SSFI_Ex& e) {
        log_err(LOC, "%s thrown:", typeid(e).name());
        e.err();
        ret = RetCode::SSFI;
    } catch (const std::exception& e) {
        log_err(LOC, "%s thrown: %s", typeid(e).name(), e.what());
        ret = RetCode::GENERIC;
    }

    return static_cast<int>(ret);
}

void filer(std::string path) {
    DIR *dir = nullptr;

    try {

        struct stat file_info;
        if (stat(path.c_str(), &file_info) == -1) {
            throw SSFI_Ex(LOC, (const char*) nullptr, "Error stat-ing \"%s\"",
                    path.c_str());
        }

        if (S_ISDIR(file_info.st_mode)) {

            log(LOC, "processing directory %s", path.c_str());

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

        } else if ((path.length() >= suffix.length())
                && (path.compare(path.length() - suffix.length(),
                        suffix.length(), suffix) == 0)) {
            log(LOC, "found .txt file: %s", path.c_str());

            std::unique_lock<std::mutex> lck {mx};
            log(LOC, "pushing file: %s", path.c_str());
            files.push_back(move(path));
            lck.unlock();
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

void worker(const int& id) {
    log(LOC, "worker %d: starting", id);

    std::string file;
    bool process_file = false;
    bool wl = true;

    do {

        process_file = false;
        std::unique_lock<std::mutex> lck { mx };
        if (!files.empty()) {
            process_file = true;
            file.assign(files.front());
            files.pop_front();
        } else {
            wl = !fin;
        }
        lck.unlock();

        if (process_file) {
            log(LOC, "processing file: %s", file.c_str());
            //sleep(3);
        }
    } while (wl);

    log(LOC, "worker %d: ending", id);
}

