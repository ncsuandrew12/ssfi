#include "counter.h"

#include <exception>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include "log.h"
#include "reader.h"

Counter::Counter(const int& id, Queue* files): _files(files), _idx(id) {
    _thread = new std::thread( [this] { this->run(); } );
}

Counter::~Counter() {
    join();
}

std::map<std::string, long>::iterator Counter::begin() {
    return _words.begin();
}

std::map<std::string, long>::iterator Counter::end() {
    return _words.end();
}

int Counter::index() {
    return _idx;
}

void Counter::join() {
    if (_thread != nullptr) {
        _thread->join();
        delete _thread;
        _thread = nullptr;
    }
}

void Counter::process_file(std::string file) {
    log(LOC, "processing file: %s", file.c_str());

    std::ifstream stream(file.c_str());
    std::string word;

    Reader reader(&stream);

    while (reader >> word) {
        log(LOC, "found word \"%s\" in file: %s", word.c_str(), file.c_str());
        _words[word]++;
    }
}

void Counter::run() {
    log(LOC, "worker %d: starting", _idx);

    try {
        _words.clear();

        bool active = true;
        do {
            std::string file;

            bool rc = _files->pop(&file);

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
                active = !_files->is_done();
            }
        } while (active);

    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        _exp = std::current_exception();
    }

    log(LOC, "worker %d: ending", _idx);
}
