#ifndef DIR_COUNTER_H_
#define DIR_COUNTER_H_

#include <list>
#include <string>
#include <mutex>

class Dir_Counter {
public:
    explicit Dir_Counter(const Dir_Counter& dc);
    explicit Dir_Counter(const int& workers, std::string dir_path);

    ~Dir_Counter();

    void run();

    bool pop_file(std::string* file);

private:
    std::list<std::string> _files;
    bool _fin = false;
    std::mutex* _mx;
    std::string _suffix = std::string(".txt");
    const int _workers;
    std::string _dir_path;

    void filer();
    void filer(std::string dir_path);
};

#endif /* DIR_COUNTER_H_ */