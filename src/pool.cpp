#include "pool.h"

#include <iterator>
#include <vector>

Pool::Pool() {
}

Pool::~Pool() {
    /*
     * Clean up the worker thread objects.
     */
    for (auto iter = m_counters.begin(); iter != m_counters.end(); iter++) {
        delete (*iter);
    }
    m_counters.clear();
}

std::vector<Counter*>::iterator Pool::begin() {
    return m_counters.begin();
}

std::vector<Counter*>::iterator Pool::end() {
    return m_counters.end();
}

void Pool::push(Counter* counter) {
    m_counters.push_back(counter);
}

