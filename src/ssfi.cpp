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
        int worker_threads = -1;
        std::string path_arg;

        /*
         * Parse CLI options.
         */
        for (int i = 1; i < argc; i++) {
            if (std::string(argv[i]) == "-t") {
                if (i >= argc - 1) {
                    throw std::invalid_argument(
                            printf_to_string("no value provided for \'%s\'",
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
            } else if (path_arg.empty()) {
                path_arg = std::string(argv[i]);
            } else {
                log(LOC, "Ignoring extraneous arg #%d: %s", i, argv[i]);
            }
        }

        if (path_arg.empty()) {
            throw std::invalid_argument("no path provided");
        }

        /*
         * Run the logic.
         */
        Dir_Counter(worker_threads, path_arg).run();

        log(LOC, "Done.");

    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
        ret = EINVAL;
    } catch (const std::system_error& e) {
        std::cout << e.what() << std::endl;
        ret = e.code().value();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        ret = ENOMSG;
    }

    return ret;
}

