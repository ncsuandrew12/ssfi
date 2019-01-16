#ifndef QUEUE_H_
#define QUEUE_H_

#include <list>
#include <string>
#include <mutex>

class Queue {
private:
    bool _done = false;
    std::list<std::string> _files;
    std::mutex* _mx;
    bool _pre_done = false;
public:
    Queue();

    ~Queue();

    /*
     * Indicate that no more items will be added to the queue.
     */
    void done();

    /*
     * Return a boolean indicating whether the queue is empty AND master thread
     * has indicated file indexing is complete.
     */
    bool is_done();

    /*
     * Pop a file off the queue of to-be-processed files.
     */
    bool pop(std::string* item);

    /*
     * Push a file to the queue of to-be-processed files.
     * Do not call after calling done().
     */
    void push(std::string item);
};

#endif /* QUEUE_H_ */
