#ifndef ERR_H_
#define ERR_H_

#include <stdio.h>

#include <stdexcept>
#include <string>

class SSFI_Ex: public std::exception {
private:
    std::exception* _cause;
    const char* _file;
    const char* _func;
    int _line;
    std::string _msg;
    const char* _what;

public:
    explicit SSFI_Ex(const char* file, const int& line, const char* func,
            const char* what, const char* format, ...);
    explicit SSFI_Ex(const char* file, const int& line, const char* func,
            std::exception* cause, const char* what, const char* format, ...);

    virtual ~SSFI_Ex();
    void err() const;
    std::string msg() const;
    void print(FILE* file) const;
    const char * what() const noexcept;
};

#endif /* ERR_H_ */
