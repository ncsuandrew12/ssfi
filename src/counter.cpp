#include "counter.h"

#include <exception>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include "log.h"
#include "reader.h"

Counter::Counter(const int& id, Queue* files): m_files(files), m_idx(id) {
    m_thread = new std::thread( [this] { this->run(); } );
}

Counter::~Counter() {
    join();
}

std::map<std::string, long>::iterator Counter::begin() {
    return m_words.begin();
}

std::map<std::string, long>::iterator Counter::end() {
    return m_words.end();
}

int Counter::index() {
    return m_idx;
}

void Counter::join() {
    if (m_thread != nullptr) {
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }
}

void Counter::process_file(std::string file) {
    log(LOC, "processing file: %s", file.c_str());

    std::ifstream stream(file.c_str());
    std::string word;

    Reader reader(&stream);

    while (reader >> word) {
        log(LOC, "found word \"%s\" in file: %s", word.c_str(), file.c_str());
        m_words[word]++;
    }
}

void Counter::run() {
    log(LOC, "worker %d: starting", m_idx);

    try {
        m_words.clear();

        bool active = true;
        do {
            std::string file;

            bool rc = m_files->pop(&file);

            if (rc) {
                process_file(file);
            } else {
                /*
                 * There aren't currently any files to process.
                 *
                 * If the indexer has indicated indexing is complete or that
                 * it's been killed, then we're done. Otherwise, wait until
                 * another file gets added or the done flag gets set.
                 */
                active = !m_files->is_done();
            }
        } while (active);

    } catch (const std::exception& e) {
        m_exp = std::current_exception();
    }

    log(LOC, "worker %d: ending", m_idx);
}
