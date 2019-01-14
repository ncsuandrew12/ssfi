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
