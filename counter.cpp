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
//        log(LOC, "found word \"%s\" in file: %s", word.c_str(), path.c_str());
        _words[word]++;
        word.clear();
    }

//    for (auto wi = _words.begin(); wi != _words.end(); wi++) {
//        log(LOC, "word \"%s\": %d instances", wi->first.c_str(), wi->second);
//    }
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
        // RetCode::SSFI;
    } catch (const std::exception& e) {
        log_err(LOC, "%s thrown: %s", typeid(e).name(), e.what()); // TODO
        // RetCode::GENERIC;
    }

    log(LOC, "worker %d: ending", _id);
}
