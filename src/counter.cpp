#include "counter.h"

#include <map>
#include <mutex>
#include <string>
#include <thread>

#include "dir_counter.h"
#include "log.h"
#include "reader.h"
#include "ssfi_ex.h"

Counter::Counter(const Counter& c) :
        _dir_cnt(c._dir_cnt), _err(c._err), _id(c._id), _thread(c._thread), _words(
                c._words) {
}

Counter::Counter(const int& id, Dir_Counter* dc): _id(id), _dir_cnt(dc) {
    _thread = new std::thread( [this] { this->run(); } );
}

Counter::~Counter() {
    join();
    delete _thread;
}

void Counter::join() {
    if (*_thread.joinable()) {
        *_thread.join();
    }
}

void Counter::process_file(std::string path) {
    log(LOC, "processing file: %s", path.c_str());

    std::ifstream stream(path.c_str());
    std::string word;

    Reader reader(&stream);

    while (reader >> word) {
        log(LOC, "found word \"%s\" in file: %s", word.c_str(), path.c_str());
        _words[word]++;
    }
}

void Counter::run() {
    log(LOC, "worker %d: starting", _id);

    try {
        std::string file;
        bool done = false;
        bool active = true;
        _words.clear();

        do {
            file.clear();

            done = _dir_cnt->pop_file(&file);

            if (!file.empty()) {
                process_file(file);
            } else {
                active = !done;
            }
        } while (active);

    } catch (const Ssfi_Ex& e) {
        e.err(LOC);
        _err(Ssfi_Ex(LOC, (const char*) e.what(), "%s", e.msg().c_str()));
    } catch (const std::exception& e) {
        log_err(LOC, "%s thrown: %s", typeid(e).name(), e.what());
        _err(Ssfi_Ex(LOC, (const char*) e.what(), (const char*) nullptr));
    }

    log(LOC, "worker %d: ending", _id);
}
