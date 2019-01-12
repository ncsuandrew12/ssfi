#ifndef ERR_H_
#define ERR_H_

#include <stdio.h>

#include <stdexcept>
#include <string>

class SSFI_Ex: public std::exception {
    std::exception* _cause;
    const char* _file;
    const char* _func;
    int _line;
    std::string _msg;
    const char* _what;

public:
    explicit SSFI_Ex(const char* file, const int& line, const char* func,
            std::exception* cause);
    explicit SSFI_Ex(const char* file, const int& line, const char* func,
            std::exception* cause, const char* format, ...);
    explicit SSFI_Ex(const char* what, const char* file, const int& line,
            const char* func, const char* format, ...);
    explicit SSFI_Ex(const char* what, const char* file, const int& line,
            const char* func, std::exception* cause, const char* format, ...);

    virtual ~SSFI_Ex();
    void err() const noexcept;
    std::string msg() const noexcept;
    void print(FILE* file) const noexcept;
    const char * what() const noexcept;
};

#endif /* ERR_H_ */
