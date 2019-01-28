#ifndef DIR_COUNTER_H_
#define DIR_COUNTER_H_

#include <exception>
#include <list>
#include <string>
#include <mutex>

#include "queue.h"

class Dir_Counter {
private:
    const std::string m_dir_path;
    std::exception_ptr m_exp;
    Queue m_files;
    std::string m_suffix = std::string(".txt");
    const int m_workers;

    /*
     * Index all files, including m_dir_path and any children.
     */
    void filer();

    /*
     * Index all files, including m_dir_path and any children.
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
