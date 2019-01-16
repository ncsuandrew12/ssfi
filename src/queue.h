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
     * Return a boolean indicating whether the queue is empty AND the producer
     * has indicated that no more items will be added.
     */
    bool is_done();

    /*
     * Indicate that no more items will be added to the queue.
     *
     * push() should never be called after this.
     */
    void last_push();

    /*
     * Pop a file off the queue of to-be-processed files.
     *
     * Return true if an item was popped, false if it wasn't.
     */
    bool pop(std::string* item);

    /*
     * Push a file to the queue of to-be-processed files.
     *
     * Do not call after calling done(). Doing so produces an exception.
     */
    void push(std::string item);
};

#endif /* QUEUE_H_ */
