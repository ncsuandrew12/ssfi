#include <stdio.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "log.h"

int main(int argc, char **argv) {
    try {
    int worker_threads = 1;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-t") {
            if (i >= argc - 1) {
                throw "no value provided for ";
            }
            worker_threads = std::stoi(argv[i+1]);
        }
    }

    log("worker threads: %d\n", worker_threads);

    } catch(const std::invalid_argument& e) {
        fprintf(stderr, "Exception thrown: %s\n", e.what());
    }
    return 0;
}

