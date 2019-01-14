#ifndef COUNTER_H_
#define COUNTER_H_

#include <map>
#include <string>
#include <thread>

#include "dir_counter.h"

class Counter {
public:
    const int _id;
    std::thread _thread;

    Counter(const int& id, Dir_Counter* dc);
    void join();
    void run();

private:
    Dir_Counter* _dc;
    std::map<std::string, long> _words;

    void process_file(std::string path);
};

#endif /* COUNTER_H_ */