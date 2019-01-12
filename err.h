#ifndef ERR_H_
#define ERR_H_

#include <stdarg.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>

#include "util.h"

#define EX_P2S_MAX 512

#define SSFI_EX(...) SSFI_Ex(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

class SSFI_Ex: public std::exception {
    std::exception* _cause;
    const char* _file;
    const char* _func;
    int _line;
    std::string _msg;
    const char* _what;

public:
    explicit SSFI_Ex(const char* file, const char* func, const int& line,
            std::exception* cause, const char* format, const int& argc, ...);
    explicit SSFI_Ex(const char* file, const char* func, const int& line,
            const char* what, const char* format, const int& argc, ...);
    explicit SSFI_Ex(const char* file, const char* func, const int& line,
            std::exception* cause, const char* what, const char* format,
            const int& argc, ...);
    explicit SSFI_Ex(const char* file, const char* func, const int& line,
            std::exception* cause);

    virtual ~SSFI_Ex();
    void err() const noexcept;
    void print(FILE* stream) const noexcept;
    const char * what() const noexcept;
};

#endif /* ERR_H_ */
