#include <stdio.h>

#include <dirent.h>
#include <sys/stat.h>

#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include "err.h"
#include "log.h"

enum class RetCode {
    SUCCESS = 0x00,
    SSFI = 0x01,
    GENERIC = 0x02
};

void process_path(std::list<std::string> files, std::string path);

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
            } else if (dir_arg == "") {
                dir_arg = std::string(argv[i]);
            } else {
                log(LOC, "Ignoring extraneous arg #%d: %s", i, argv[i]);
            }
        }

        if (dir_arg == "") {
            throw SSFI_Ex(LOC,
                    new std::invalid_argument("No target directory provided"),
                    (const char*) nullptr, (const char*) nullptr);
        }

        log(LOC, "worker threads: %d", worker_threads);
        log(LOC, "target directory: %s", dir_arg.c_str());

        std::list<std::string> files;
        process_path(files, dir_arg);

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

void process_path(std::list<std::string> files, std::string path) {
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

                if ((std::string(de->d_name).compare(".") == 0)
                        || (std::string(de->d_name).compare("..") == 0)) {
                    continue;
                }

                std::string subdir_path = std::string(path);
                subdir_path.append("/");
                subdir_path.append(de->d_name);

                log(LOC, "child path: %s", subdir_path.c_str());

                process_path(files, subdir_path);
            }

        } else {

            log(LOC, "found file %s", path.c_str());
            files.push_back(path);

        }

        closedir(dir);
    } catch (const std::exception& e) {
        if (dir != nullptr) {
            closedir(dir);
        }
        throw e;
    }

}

