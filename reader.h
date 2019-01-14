#ifndef READER_H_
#define READER_H_

#include <stdio.h>

#include <fstream>
#include <string>

#include "log.h"

class Reader {
    std::ifstream* _stream;
public:
    Reader();
    Reader(const Reader& reader);
    Reader(std::ifstream* stream);

    bool operator >>(std::string& out) {
        out.clear();
        bool begin = false;
        char c;
        while (EOF != (c = _stream->get())) {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
                    || (c >= '0' && c <= '9')) {
                begin = true;
                out += c;
            } else if (begin) {
                return true;
            }
        };
        return false;
    }
};

#endif /* READER_H_ */
