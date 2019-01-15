#include "reader.h"

#include <fstream>
#include <string>

#include "log.h"

Reader::Reader() :
        _stream(nullptr) {
}

Reader::Reader(const Reader& reader) :
        _stream(reader._stream) {
}

Reader::Reader(std::ifstream* stream) :
        _stream(stream) {
}

bool Reader::operator >>(std::string& out) {
    out.clear();
    bool begun = false;
    char c;
    while (EOF != (c = _stream->get())) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                || (c >= '0' && c <= '9')) {
            begun = true;
            out += c;
        } else if (begun) {
            return true;
        }
    };
    return false;
}
