#include <stdio.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#include "err.h"
#include "log.h"

int main(int argc, char **argv) {
    try {
        int worker_threads = 1;

        for (int i = 1; i < argc; i++) {
            if (std::string(argv[i]) == "-t") {
                if (i >= argc - 1) {
                    throw SSFI_Ex(LOC,
                            new std::invalid_argument("no value provided"),
                            "no value provided for %s", argv[i]);
                }
                worker_threads = std::stoi(argv[i + 1]);
            }
        }

        log(LOC, "worker threads: %d", worker_threads);

    } catch (const SSFI_Ex& e) {
        e.err();
    } catch (const std::invalid_argument& e) {
        log_err(LOC, "%s thrown: %s", typeid(e).name(), e.what());
    }
    return 0;
}

