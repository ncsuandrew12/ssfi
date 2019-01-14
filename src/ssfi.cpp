#include <stdio.h>

#include <stdexcept>
#include <string>
#include <typeinfo>

#include "dir_counter.h"
#include "err.h"
#include "log.h"

enum class RetCode {
    SUCCESS = 0x00,
    SSFI = 0x01,
    GENERIC = 0x02
};

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

        Dir_Counter(worker_threads, dir_arg).run();

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

