#include "queue.h"

#include <mutex>

#include "log.h"
#include "queue.h"

Queue::Queue(const Queue& other) :
        _done(other._done), _files(other._files), _mx(other._mx) {
}

Queue::Queue() {
    _mx = new std::mutex();
}

Queue::~Queue() {
    delete _mx;
}

/*
 * Indicate that no more items will be added to the queue.
 */
void Queue::done() {
    std::lock_guard<std::mutex> lck { *_mx };
    _pre_done = true;
}

/*
 * Return a boolean indicating whether the master thread has indicated file
 * indexing is complete.
 */
bool Queue::is_done() {
    std::lock_guard<std::mutex> lck { *_mx };
    return _done;
}

/*
 * Pop a file off the queue of to-be-processed files.
 */
std::string Queue::pop() {
    std::string ret;
    std::lock_guard<std::mutex> lck { *_mx };
    if (!_files.empty()) {
        ret.assign(_files.front());
        log(LOC, "popped %s", ret.c_str());
        _files.pop_front();
    }
    if (_pre_done && _files.empty()) {
        _done = true;
    }
    return ret;
}

/*
 * Push a file to the queue of to-be-processed files.
 */
void Queue::push(std::string str) {
    std::lock_guard<std::mutex> lck { *_mx };
    log(LOC, "pushing file: %s", str.c_str());
    _files.push_back(std::move(str));
}

