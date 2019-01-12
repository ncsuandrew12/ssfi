#include "err.h"

#include <stdarg.h>
#include <stdio.h>

#include <string>

SSFI_Ex::SSFI_Ex(const char* file, const char* func, const int& line,
        std::exception* cause, const char* format, const int& argc, ...) :
        _cause(cause), _file(file), _func(func), _line(_line), _what(nullptr) {
    if (format != nullptr) {
        va_list args;
        va_start(args, argc);
        this->_msg = print_to_string(EX_P2S_MAX, format, args);
        va_end(args);
    }
}

SSFI_Ex::SSFI_Ex(const char* file, const char* func, const int& line,
        const char* msg, const char* format, const int& argc, ...) :
        _cause(nullptr), _file(file), _func(func), _line(_line), _what(msg) {
    if (format != nullptr) {
        va_list args;
        va_start(args, argc);
        this->_msg = print_to_string(EX_P2S_MAX, format, args);
        va_end(args);
    }
}

SSFI_Ex::SSFI_Ex(const char* file, const char* func, const int& line,
        std::exception* cause, const char* msg, const char* format,
        const int& argc, ...) :
        _cause(cause), _file(file), _func(func), _line(_line), _what(msg) {
    va_list args;

    if (format != nullptr) {
        va_list args;
        va_start(args, argc);
        this->_msg = print_to_string(EX_P2S_MAX, format, args);
        va_end(args);
    }
}

SSFI_Ex::SSFI_Ex(const char* file, const char* func, const int& line,
        std::exception* cause) :
        _cause(cause), _file(file), _func(func), _line(_line), _msg(nullptr), _what(
                nullptr) {
}

SSFI_Ex::~SSFI_Ex() {
    if (_cause != nullptr) {
        delete _cause;
    }
}

void SSFI_Ex::err() const noexcept {
    print(stderr);
}

void SSFI_Ex::print(FILE* stream) const noexcept {
    int ret = 0;
    std::string str;
    fprintf(stream, "%s(%s:%d) %s: ", _func, _file, _line, typeid(this).name());
    if (_msg != "") {
        fprintf(stream, _msg.c_str());
    } else if (_what != nullptr) {
        fprintf(stream, "%s", _what);
    }
    if (_cause != nullptr) {
        fprintf(stream, "\n\tcaused by: ");
        if (dynamic_cast<SSFI_Ex*>(_cause)) {
            dynamic_cast<SSFI_Ex*>(_cause)->print(stream);
        } else {
            fprintf(stream, "%s: %s\n", typeid(_cause).name(), _cause->what());
        }
    } else {
        fprintf(stream, "\n");
    }
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
