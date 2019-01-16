#ifndef DIR_COUNTER_H_
#define DIR_COUNTER_H_

#include <exception>
#include <list>
#include <string>
#include <mutex>

#include "queue.h"

class Dir_Counter {
public:
    Queue _files;

    explicit Dir_Counter(const int& workers, const std::string dir_path);

    void run();

private:
    std::exception_ptr _exp;
    std::string _suffix = std::string(".txt");
    const int _workers;
    const std::string _dir_path;

    void filer();
    void filer(const std::string dir_path);
};

#endif /* DIR_COUNTER_H_ */
