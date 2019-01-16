#ifndef READER_H_
#define READER_H_

#include <fstream>
#include <string>

class Reader {
private:
    std::ifstream* _stream;
public:
    explicit Reader(std::ifstream* stream);

    /*
     * Set 'out' to the next word (converted to lowercase).
     *
     * Return a boolean indicating whether the stream can continue to be read
     * (true if the stream hasn't ended; false if the stream has ended).
     */
    bool operator >>(std::string& out);
};

#endif /* READER_H_ */
