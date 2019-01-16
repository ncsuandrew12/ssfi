#ifndef COUNTER_H_
#define COUNTER_H_

#include <exception>
#include <map>
#include <string>
#include <thread>

#include "dir_counter.h"

class Counter {
private:
    std::thread* _thread;
    Queue* _files;

    void process_file(std::string path);

public:
    std::exception_ptr _exp;
    const int _id;
    std::map<std::string, long> _words;

    Counter(const int& id, Queue* files);
    ~Counter();

    void join();
    void run();
};

#endif /* COUNTER_H_ */
