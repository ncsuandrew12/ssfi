#include "queue.h"

#include <mutex>
#include <stdexcept>

#include "log.h"
#include "queue.h"

Queue::Queue() {
    _mx = new std::mutex();
}

Queue::~Queue() {
    delete _mx;
}

void Queue::done() {
    std::lock_guard<std::mutex> lck { *_mx };
    _pre_done = true;
}

bool Queue::is_done() {
    std::lock_guard<std::mutex> lck { *_mx };
    if (_pre_done && _files.empty()) {
        _done = true;
    }
    return _done;
}

bool Queue::pop(std::string* item) {
    bool ret = false;
    std::lock_guard<std::mutex> lck { *_mx };
    if (!_files.empty()) {
        ret = true;
        item->assign(_files.front());
        log(LOC, "popped %s", item->c_str());
        _files.pop_front();
    }
    if (_pre_done && _files.empty()) {
        _done = true;
    }
    return ret;
}

void Queue::push(std::string item) {
    std::lock_guard<std::mutex> lck { *_mx };
    if (_done) {
        throw std::logic_error("Queue: push() called after done()");
    }
    log(LOC, "pushing file: %s", item.c_str());
    _files.push_back(std::move(item));
}

