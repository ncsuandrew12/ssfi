#include "pool.h"

#include <iterator>

Pool::Pool() {
}

Pool::~Pool() {
    /*
     * Clean up the worker thread objects.
     */
    for (auto iter = _counters.begin(); iter != _counters.end(); iter++) {
        delete (*iter);
    }
    _counters.clear();
}

