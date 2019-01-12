#include "err.h"

#include <stdarg.h>

#include <memory>
#include <typeinfo>
#include <string>

#include "util.h"

SSFI_Ex::SSFI_Ex(const char* file, const int& line, const char* func,
        std::exception* cause) :
        _cause(cause), _file(file), _func(func), _line(_line), _msg(nullptr), _what(
                nullptr) {
}

SSFI_Ex::SSFI_Ex(const char* file, const int& line, const char* func,
        std::exception* cause, const char* format, ...) :
        _cause(cause), _file(file), _func(func), _line(_line), _what(nullptr) {
    if (format != nullptr) {
        va_list args;
        va_start(args, format);
        size_t size = printf_len(format, args);
        va_end(args);
        va_start(args, format);
        this->_msg = printf_to_string(size, format, args);
        va_end(args);
    }
}

SSFI_Ex::SSFI_Ex(const char* w, const char* file, const int& line,
        const char* func, const char* format, ...) :
        _cause(nullptr), _file(file), _func(func), _line(_line), _what(w) {
    if (format != nullptr) {
        va_list args;
        va_start(args, format);
        size_t size = printf_len(format, args);
        va_end(args);
        va_start(args, format);
        this->_msg = printf_to_string(size, format, args);
        va_end(args);
    }
}

SSFI_Ex::SSFI_Ex(const char* w, const char* file, const int& line,
        const char* func, std::exception* cause, const char* format, ...) :
        _cause(cause), _file(file), _func(func), _line(_line), _what(w) {
    printf(LOC_PRINTF, LOC);
    printf("\n");
    if (format != nullptr) {
        printf(LOC_PRINTF, LOC);
        printf("\n");
        va_list args;
        va_start(args, format);
        printf(LOC_PRINTF, LOC);
        printf("format=%s\n", format);
        size_t size = printf_len(format, args);
        printf(LOC_PRINTF, LOC);
        printf("size=%d\n", size);
        va_end(args);
        va_start(args, format);
        this->_msg = printf_to_string(size, format, args);
        printf(LOC_PRINTF, LOC);
        printf("_msg=%s\n", _msg.c_str());
        va_end(args);
    }
}

SSFI_Ex::~SSFI_Ex() {
    if (_cause != nullptr) {
        delete _cause;
    }
}

void SSFI_Ex::err() const noexcept {
    print(stderr);
}

std::string SSFI_Ex::msg() const noexcept {
    std::string ret = printf_to_string(LOC_PRINTF, _file, _line, _func);
    ret.append(printf_to_string("%s: ", typeid(this).name()));
    if (_msg != "") {
        ret.append(_msg.c_str());
    } else if (_what != nullptr) {
        ret.append(_what);
    }
    if (_cause != nullptr) {
        ret.append("\n\tcaused by: ");
        if (dynamic_cast<SSFI_Ex*>(_cause)) {
            ret.append(dynamic_cast<SSFI_Ex*>(_cause)->msg());
        } else {
            ret.append(_cause->what());
        }
    }
    return ret;
}

void SSFI_Ex::print(FILE* file) const noexcept {
    fprintf(file, "%s\n", msg().c_str());
}

const char* SSFI_Ex::what() const noexcept {
    if (_what != nullptr) {
        return _what;
    }
    if (_cause != nullptr) {
        return _cause->what();
    }
    return "unknown";
}
