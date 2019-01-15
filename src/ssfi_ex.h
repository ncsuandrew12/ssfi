#ifndef SSFI_EX_H_
#define SSFI_EX_H_

#include <stdio.h>

#include <stdexcept>
#include <string>

class Ssfi_Ex: public std::exception {
private:
    const char* _file;
    const char* _func;
    int _line;
    std::string _msg;
    const char* _what;

public:
    Ssfi_Ex();
    Ssfi_Ex(const Ssfi_Ex& other);
    Ssfi_Ex(const char* file, const int& line, const char* func,
            const char* what, const char* format, ...);

    void err(const char* file, const int& line, const char* func) const;
    std::string msg() const;
    Ssfi_Ex& operator=(const Ssfi_Ex& other);
    void print(FILE* f, const char* file, const int& line,
            const char* func) const;
    std::string summary() const;
    const char * what() const noexcept;
};

#endif /* SSFI_EX_H_ */
