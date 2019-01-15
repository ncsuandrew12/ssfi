#ifndef READER_H_
#define READER_H_

#include <fstream>
#include <string>

#include "log.h"

class Reader {
    std::ifstream* _stream;
public:
    Reader();
    Reader(const Reader& reader);
    Reader(std::ifstream* stream);

    bool operator >>(std::string& out);
};

#endif /* READER_H_ */
