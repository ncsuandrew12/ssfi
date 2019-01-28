#ifndef COUNTER_H_
#define COUNTER_H_

#include <exception>
#include <iterator>
#include <map>
#include <string>
#include <thread>

#include "queue.h"

class Counter {
private:
    Queue* m_files;
    const int m_idx;
    std::thread* m_thread;
    std::map<std::string, long> m_words;

    /*
     * Process the given file, counting the instances of each word it contains.
     */
    void process_file(const std::string file);

public:
    std::exception_ptr m_exp;

    explicit Counter(const int& id, Queue* files);
    ~Counter();

    /*
     * Return an iterator pointing at the first counter.
     */
    std::map<std::string, long>::iterator begin();

    /*
     * Return an iterator pointing past the last counter.
     */
    std::map<std::string, long>::iterator end();

    /*
     * Return the index of this counter.
     */
    int index();

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
