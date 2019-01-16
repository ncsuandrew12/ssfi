#include "pool.h"

#include <iterator>
#include <vector>

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

std::vector<Counter*>::iterator Pool::begin() {
    return _counters.begin();
}

std::vector<Counter*>::iterator Pool::end() {
    return _counters.end();
}

void Pool::push(Counter* counter) {
    _counters.push_back(counter);
}

