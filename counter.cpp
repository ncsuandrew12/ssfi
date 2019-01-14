#include "counter.h"

#include <map>
#include <mutex>
#include <string>
#include <thread>

#include "err.h"
#include "dir_counter.h"
#include "log.h"
#include "reader.h"

Counter::Counter(const int& id, Dir_Counter* dc): _id(id), _dc(dc) {
    _thread = std::thread( [this] { this->run(); } );
}

void Counter::join() {
    _thread.join();
}

void Counter::process_file(std::string path) {
    log(LOC, "processing file: %s", path.c_str());

    std::ifstream stream(path.c_str());
    std::string word;

    Reader reader(&stream);

    while (reader >> word) {
        log(LOC, "found word \"%s\" in file: %s", word.c_str(), path.c_str());
        _words[word]++;
        word.clear();
    }
}

void Counter::run() {
    log(LOC, "worker %d: starting", _id);

    try {
        std::string file;
        bool fin = false;
        bool wl = true;
        _words.clear();

        do {
            file.clear();

            fin = _dc->pop_file(&file);

            if (!file.empty()) {
                process_file(file);
            } else {
                wl = !fin;
            }
        } while (wl);

    } catch (const SSFI_Ex& e) {
        log_err(LOC, "%s thrown:", typeid(e).name());
        e.err(); // TODO
        _err = new SSFI_Ex(LOC, (std::exception*) nullptr,
                (const char*) e.what(), "%s", e.msg().c_str());
    } catch (const std::exception& e) {
        log_err(LOC, "%s thrown: %s", typeid(e).name(), e.what());
        _err = new SSFI_Ex(LOC, (std::exception*) nullptr,
                (const char*) e.what(), (const char*) nullptr);
    }

    log(LOC, "worker %d: ending", _id);
}
