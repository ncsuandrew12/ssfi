#include "reader.h"

#include <fstream>
#include <string>

Reader::Reader(std::ifstream* stream) :
        m_stream(stream) {
}

bool Reader::operator >>(std::string& out) {
    out.clear();
    bool begun = false;
    char c;
    while (EOF != (c = m_stream->get())) {
        if (c >= 'A' && c <= 'Z') {
            c += 'a' - 'A';
        } else if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))) {
            if (begun) {
                return true;
            } else {
                continue;
            }
        }
        begun = true;
        out += c;
    };
    return false;
}
