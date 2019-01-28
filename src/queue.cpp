#include "queue.h"

#include <mutex>
#include <stdexcept>

#include "log.h"

Queue::Queue() {
    m_mx = new std::mutex();
}

Queue::~Queue() {
    delete m_mx;
}

bool Queue::is_done() {
    std::lock_guard<std::mutex> lck { *m_mx };
    if (m_killed || (m_pre_done && m_list.empty())) {
        m_done = true;
    }
    return m_done;
}

void Queue::kill() {
    std::lock_guard<std::mutex> lck { *m_mx };
    m_killed = true;
}

void Queue::last_push() {
    std::lock_guard<std::mutex> lck { *m_mx };
    m_pre_done = true;
}

bool Queue::pop(std::string* item) {
    bool ret = false;
    std::lock_guard<std::mutex> lck { *m_mx };
    if (m_killed) {
        throw std::system_error(
                std::error_code(1, std::generic_category()),
                "queue was killed.");
        return true;
    }
    if (!m_list.empty()) {
        ret = true;
        item->assign(m_list.front());
        log(LOC, "popped %s (%d remaining)", item->c_str(), m_list.size());
        m_list.pop_front();
    }
    if (m_pre_done && m_list.empty()) {
        m_done = true;
    }
    return ret;
}

void Queue::push(std::string item) {
    std::lock_guard<std::mutex> lck { *m_mx };
    if (m_killed) {
        throw std::system_error(
                std::error_code(1, std::generic_category()),
                "queue was killed.");
    }
    if (m_done) {
        throw std::logic_error("queue: push() called after done()");
    }
    log(LOC, "pushing file: %s", item.c_str());
    m_list.push_back(std::move(item));
}

