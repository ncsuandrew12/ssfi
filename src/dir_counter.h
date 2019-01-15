#ifndef DIR_COUNTER_H_
#define DIR_COUNTER_H_

#include <list>
#include <string>
#include <mutex>

#include "queue.h"
#include "ssfi_ex.h"

class Dir_Counter {
public:
    Queue _files;

    explicit Dir_Counter(const Dir_Counter& dc);
    explicit Dir_Counter(const int& workers, std::string dir_path);

    void run();

private:
    bool _err = false;
    Ssfi_Ex _ex;
    std::string _suffix = std::string(".txt");
    const int _workers;
    std::string _dir_path;

    void filer();
    void filer(std::string dir_path);
};

#endif /* DIR_COUNTER_H_ */
