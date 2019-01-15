#ifndef COUNTER_H_
#define COUNTER_H_

#include <map>
#include <string>
#include <thread>

#include "dir_counter.h"
#include "ssfi_ex.h"

class Counter {
public:
    Ssfi_Ex _err;
    const int _id;
    std::thread* _thread;
    std::map<std::string, long> _words;

    Counter(const Counter& dc);
    Counter(const int& id, Dir_Counter* dc);
    ~Counter();

    void join();
    void run();

private:
    Dir_Counter* _dir_cnt;

    void process_file(std::string path);
};

#endif /* COUNTER_H_ */
