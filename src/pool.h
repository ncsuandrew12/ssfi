#ifndef POOL_H_
#define POOL_H_

#include <iterator>
#include <vector>

#include "counter.h"

class Pool {
private:
    std::vector<Counter*> _counters;

public:
    Pool();
    ~Pool();

    /*
     * Return an iterator pointing at the first counter.
     */
    std::vector<Counter*>::iterator begin();

    /*
     * Return an iterator pointing past the last counter.
     */
    std::vector<Counter*>::iterator end();

    /*
     * Add a counter to the end of the vector.
     */
    void push(Counter* counter);
};

#endif /* POOL_H_ */
