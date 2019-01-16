#include <errno.h>
#include <sys/stat.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "dir_counter.h"
#include "log.h"
#include "util.h"

int main(int argc, char **argv) {
    int ret = 0;

    try {
        std::string path_arg;
        int worker_threads = 0;

        /*
         * Parse CLI options.
         */
        for (int i = 1; i < argc; i++) {
            std::string arg = std::string(argv[i]);
            if ((arg.size() > 1) && (arg.front() == '-')) {
                if (arg == "-t") {
                    if (i >= argc - 1) {
                        throw std::invalid_argument(
                                printf_to_string(
                                        "no value provided for option \'%s\'",
                                        argv[i]));
                    }
                    i++;
                    std::istringstream iss(std::string(argv[i]));
                    iss >> worker_threads;
                    if (worker_threads < 1) {
                        throw std::invalid_argument(
                                printf_to_string(
                                        "invalid value for option \'%s\' (\'%s\'). Value must be a number >= 1.",
                                        argv[i - 1], argv[i]));
                    }
                } else {
                    throw std::invalid_argument(
                            printf_to_string("invalid option -- \'%s\'", argv[i]));
                }
            } else if (path_arg.empty()) {
                path_arg = arg;
            } else {
                throw std::invalid_argument(
                        printf_to_string("invalid option -- \'%s\'", argv[i]));
            }
        }

        if (path_arg.empty()) {
            throw std::invalid_argument("no path provided");
        }

        struct stat file_info;
        if (stat(path_arg.c_str(), &file_info) == -1) {
            std::string what;
            if (errno == ENOENT) {
                what = printf_to_string("cannot access %s",
                        path_arg.c_str());
            } else {
                what = printf_to_string("error stat-ing \"%s\"",
                        path_arg.c_str());
            }
            throw std::system_error(
                    std::error_code(errno, std::generic_category()),
                    what.c_str());
        }

        if (worker_threads < 1) {
            worker_threads = 1;
        }

        /*
         * Run the logic.
         */
        Dir_Counter(worker_threads, path_arg).run();

        log(LOC, "Done.");

    } catch (const std::invalid_argument& e) {
        std::cerr << "ssfi: " << e.what() << std::endl;
        ret = EINVAL;
    } catch (const std::system_error& e) {
        std::cerr << "ssfi: " << e.what() << std::endl;
        ret = e.code().value();
    } catch (const std::exception& e) {
        std::cerr << "ssfi: " << e.what() << std::endl;
        ret = ENOMSG;
    }

    return ret;
}

