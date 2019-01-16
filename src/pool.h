#ifndef POOL_H_
#define POOL_H_

#include <vector>

#include "counter.h"

class Pool {
public:
    std::vector<Counter*> _counters;

    Pool();

    ~Pool();

};

#endif /* POOL_H_ */
