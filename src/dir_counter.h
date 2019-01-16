#ifndef DIR_COUNTER_H_
#define DIR_COUNTER_H_

#include <exception>
#include <list>
#include <string>
#include <mutex>

#include "queue.h"

class Dir_Counter {
private:
    std::exception_ptr _exp;
    Queue _files;
    std::string _suffix = std::string(".txt");
    const int _workers;
    const std::string _dir_path;

    /*
     * Index all files, including _dir_path and any children.
     */
    void filer();

    /*
     * Index all files, including dir_path and any children.
     */
    void filer(const std::string dir_path);

public:
    explicit Dir_Counter(const int& workers, const std::string dir_path);

    /*
     * Run the main SSFI logic, including launching threads.
     */
    void run();
};

#endif /* DIR_COUNTER_H_ */
