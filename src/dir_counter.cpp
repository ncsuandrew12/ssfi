#include "dir_counter.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <system_error>
#include <vector>

#include "counter.h"
#include "log.h"
#include "pool.h"
#include "util.h"
#include "queue.h"

Dir_Counter::Dir_Counter(const int& workers, const std::string dir_path) :
        _dir_path(dir_path), _workers(workers) {
}

void Dir_Counter::run() {
    log(LOC, "worker threads: %d", _workers);
    log(LOC, "target directory: %s", _dir_path.c_str());

    log(LOC, "launching filer thread");
    std::thread tfiler = std::thread([this] {this->filer();});

    Pool pool;
    for (int i = 1; i <= _workers; i++) {
        log(LOC, "launching worker thread %d", i);
        pool.push(new Counter(i, &_files));
    }

    log(LOC, "joining filer thread");
    tfiler.join();

    /*
     * Check for errors during file indexing.
     */
    if (_exp != nullptr) {
        _files.kill();
        std::rethrow_exception(_exp);
    }

    log(LOC, "signaling indexing completion");
    _files.last_push();

    std::map<std::string, long> words;

    for (auto iter = pool.begin(); iter != pool.end();
            iter++) {
        log(LOC, "joining worker thread %d", (*iter)->index());
        (*iter)->join();

        /*
         * Check for errors during the worker thread's execution.
         */
        if ((*iter)->_exp != nullptr) {
            std::rethrow_exception((*iter)->_exp);
        }

        /*
         * Merge the thread's word count map with the overall word count
         * map.
         */
        for (auto wi = (*iter)->begin(); wi != (*iter)->end(); wi++) {
            log(LOC, "word \"%s\": %d+=%d instances", wi->first.c_str(),
                    words[wi->first.c_str()], wi->second);
            words[wi->first.c_str()] += wi->second;
        }
    }

    /*
     * Find the top 10 most common words.
     */
    std::vector<std::string> most_common;
    const int mcl = 10;
    int full_smallest = 0;
    for (auto wi = words.begin(); wi != words.end(); wi++) {
        log(LOC, "word \"%s\": %d instances", wi->first.c_str(), wi->second);
        bool added = false;
        if (wi->second <= full_smallest) {
            continue;
        }
        for (auto mci = most_common.begin(); mci != most_common.end(); mci++) {
            if (wi->second > words[*mci]) {
                added = true;
                most_common.insert(mci, wi->first.c_str());
                break;
            }
        }
        if (!added && most_common.size() < mcl) {
            added = true;
            most_common.push_back(wi->first.c_str());
        }
        while (most_common.size() > mcl) {
            most_common.pop_back();
        }
        if (added && (most_common.size() == mcl)) {
            full_smallest = words[most_common.at(mcl - 1)];
        }
    }

    /*
     * Print out the top 10 most common words
     */
    int mcii = 1;
    for (auto mci = most_common.begin(); mci != most_common.end(); mci++) {
        log(LOC, "word #%02d: \"%s\": %d instances", mcii, (*mci).c_str(),
                words[*mci]);
        std::cout << (*mci).c_str() << '\t' << words[*mci] << std::endl;
        mcii++;
    }

}

/*
 * Index the files of the previously set path.
 */
void Dir_Counter::filer() {
    try {
        filer(_dir_path);
    } catch (const std::exception& e) {
        _exp = std::current_exception();
    }
}

/*
 * Queue the given path if it's a file.
 *
 * If it's a directory, index its contents.
 */
void Dir_Counter::filer(const std::string path) {
    DIR *dir = nullptr;

    try {
        struct stat file_info;
        if (stat(path.c_str(), &file_info) == -1) {
            std::string what;
            if (errno == ENOENT) {
                what = printf_to_string("cannot access %s",
                        path.c_str());
            } else {
                what = printf_to_string("error stat-ing \"%s\"", path.c_str());
            }
            throw std::system_error(
                    std::error_code(errno, std::generic_category()),
                    what.c_str());
        }

        if (S_ISDIR(file_info.st_mode)) {
            log(LOC, "reading directory %s", path.c_str());

            dir = opendir(path.c_str());
            if (dir == nullptr) {
                throw std::system_error(
                        std::error_code(EIO, std::generic_category()),
                        printf_to_string("error opening directory \"%s\"",
                                path.c_str()));
            }

            struct dirent *de;
            while ((de = readdir(dir)) != nullptr) {
                std::string name = std::string(de->d_name);

                /*
                 * Skip the relative directories.
                 */
                if ((name.compare(".") == 0) || (name.compare("..") == 0)) {
                    continue;
                }

                std::string sub_path = std::string(path);
                sub_path.append("/");
                sub_path.append(de->d_name);

                log(LOC, "child path: %s", sub_path.c_str());

                filer(sub_path);
            }
        } else if ((path.length() >= _suffix.length())
                && (path.compare(path.length() - _suffix.length(),
                        _suffix.length(), _suffix) == 0)) {
            /*
             * The current path is a .txt file. Queue it up.
             */
            log(LOC, "found %s file: %s", _suffix.c_str(), path.c_str());

            _files.push(path);
        } else {
            log(LOC, "skipping non-%s file: %s", _suffix.c_str(), path.c_str());
        }

        closedir(dir);
    } catch (const std::exception& e) {
        if (dir != nullptr) {
            closedir(dir);
        }
        throw;
    }
}

