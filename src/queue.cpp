#include "queue.h"

#include <mutex>
#include <stdexcept>

#include "log.h"

Queue::Queue() {
    _mx = new std::mutex();
}

Queue::~Queue() {
    delete _mx;
}

bool Queue::is_done() {
    std::lock_guard<std::mutex> lck { *_mx };
    if (_killed || (_pre_done && _list.empty())) {
        _done = true;
    }
    return _done;
}

void Queue::kill() {
    std::lock_guard<std::mutex> lck { *_mx };
    _killed = true;
}

void Queue::last_push() {
    std::lock_guard<std::mutex> lck { *_mx };
    _pre_done = true;
}

bool Queue::pop(std::string* item) {
    bool ret = false;
    std::lock_guard<std::mutex> lck { *_mx };
    if (_killed) {
        throw std::system_error(
                std::error_code(1, std::generic_category()),
                "Queue was killed.");
        return true;
    }
    if (!_list.empty()) {
        ret = true;
        item->assign(_list.front());
        log(LOC, "popped %s (%d remaining)", item->c_str(), _list.size());
        _list.pop_front();
    }
    if (_pre_done && _list.empty()) {
        _done = true;
    }
    return ret;
}

void Queue::push(std::string item) {
    std::lock_guard<std::mutex> lck { *_mx };
    if (_killed) {
        throw std::system_error(
                std::error_code(1, std::generic_category()),
                "Queue was killed.");
    }
    if (_done) {
        throw std::logic_error("Queue: push() called after done()");
    }
    log(LOC, "pushing file: %s", item.c_str());
    _list.push_back(std::move(item));
}

