#include <stdio.h>

#include <stdexcept>
#include <string>
#include <typeinfo>

#include "dir_counter.h"
#include "log.h"
#include "ssfi_ex.h"
#include "util.h"

int main(int argc, char **argv) {
    RetCode ret = RetCode::SUCCESS;

    try {
        int worker_threads = 1;
        std::string path_arg;

        for (int i = 1; i < argc; i++) {
            if (std::string(argv[i]) == "-t") {
                if (i >= argc - 1) {
                    throw Ssfi_Ex(LOC, "No value provided",
                            "No value provided for \"%s\"", argv[i]);
                }
                try {
                    worker_threads = std::stoi(argv[i + 1]);
                    i++; // Skip over value to next option.
                } catch (std::invalid_argument& e) {
                    throw Ssfi_Ex(LOC, "Error parsing option value.",
                            "Error parsing value for option \"%s\": \"%s\". Value must be a number.",
                            argv[i], argv[i + 1]);
                }
            } else if (path_arg.empty()) {
                path_arg = std::string(argv[i]);
            } else {
                log(LOC, "Ignoring extraneous arg #%d: %s", i, argv[i]);
            }
        }

        if (path_arg.empty()) {
            throw Ssfi_Ex(LOC, "No path provided",
                    (const char*) nullptr);
        }

        Dir_Counter(worker_threads, path_arg).run();

        log(LOC, "Done.");

    } catch (Ssfi_Ex* e) {
        e->err(LOC);
        delete e;
        ret = RetCode::ERR_GENERIC;
    } catch (const Ssfi_Ex& e) {
        e.err(LOC);
        ret = RetCode::ERR_GENERIC;
    } catch (const std::exception& e) {
        log_err(LOC, e);
        ret = RetCode::ERR_GENERIC;
    }

    return static_cast<int>(ret);
}

