#ifndef QUEUE_H_
#define QUEUE_H_

#include <list>
#include <string>
#include <mutex>

#include "ssfi_ex.h"

class Queue {
public:

    Queue();
    Queue(const Queue& dc);

    ~Queue();

    void done();
    bool is_done();
    bool pop(std::string* item);
    void push(std::string item);

private:
    bool _done = false;
    bool _pre_done = false;
    std::list<std::string> _files;
    std::mutex* _mx;

};

#endif /* QUEUE_H_ */
