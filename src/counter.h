#ifndef COUNTER_H_
#define COUNTER_H_

#include <exception>
#include <map>
#include <string>
#include <thread>

#include "queue.h"

class Counter {
private:
    std::thread* _thread;
    Queue* _files;

    /*
     * Process the given file, counting the instances of each word it contains.
     */
    void process_file(const std::string file);

public:
    std::exception_ptr _exp;
    const int _id;
    std::map<std::string, long> _words;

    explicit Counter(const int& id, Queue* files);
    ~Counter();

    /*
     * Join and free _thread
     */
    void join();

    /*
     * Run the main worker thread logic. Until the file queue is empty and no
     * more files will be added, pop files off the queue and process them.
     */
    void run();
};

#endif /* COUNTER_H_ */
