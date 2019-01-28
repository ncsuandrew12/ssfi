#ifndef QUEUE_H_
#define QUEUE_H_

#include <list>
#include <string>
#include <mutex>

class Queue {
private:
    bool m_done = false;
    bool m_killed = false;
    std::list<std::string> m_list;
    std::mutex* m_mx;
    bool m_pre_done = false;
public:
    Queue();

    ~Queue();

    /*
     * Return a boolean indicating whether:
     *  (a) the queue has been killed
     *  OR
     *   (b1) the queue is empty
     *   AND
     *   (b2) the producer as indicated that no more items will be added
     */
    bool is_done();

    /*
     * Terminate the queue.
     */
    void kill();

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
