/*
 * err.h
 *
 *  Created on: Jan 13, 2019
 *      Author: afelsher
 */

#ifndef ERR_H_
#define ERR_H_

#include <stdarg.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>

#define EX_P2S_MAX 512

#define SSFI_EX(...) SSFI_Ex(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

extern std::string print_to_string(const size_t& max, const char* format,
        va_list args);

class SSFI_Ex: public std::exception {
    std::exception* _cause;
    const char* _file;
    const char* _func;
    std::string _msg;
    int _line;
    const char* _what;

public:
    explicit SSFI_Ex(const char* file, const char* func, const int& line,
            std::exception* cause, const char* format, const int& argc, ...) {

        this->_cause = cause;
        this->_file = file;
        this->_func = func;
        this->_line = line;

        this->_what = nullptr;

        if (format != nullptr) {
            va_list args;
            va_start(args, argc);
            this->_msg = print_to_string(EX_P2S_MAX, format, args);
            va_end(args);
        }
    }
    explicit SSFI_Ex(const char* file, const char* func, const int& line,
            const char* msg, const char* format, const int& argc, ...) {

        this->_file = file;
        this->_func = func;
        this->_line = line;
        this->_what = msg;

        this->_cause = nullptr;

        if (format != nullptr) {
            va_list args;
            va_start(args, argc);
            this->_msg = print_to_string(EX_P2S_MAX, format, args);
            va_end(args);
        }
    }
    explicit SSFI_Ex(const char* file, const char* func, const int& line,
            std::exception* cause, const char* msg, const char* format,
            const int& argc, ...) {
        va_list args;

        this->_cause = cause;
        this->_file = file;
        this->_msg = format;
        this->_func = func;
        this->_line = line;
        this->_what = msg;

        if (format != nullptr) {
            va_list args;
            va_start(args, argc);
            this->_msg = print_to_string(EX_P2S_MAX, format, args);
            va_end(args);
        }
    }
    explicit SSFI_Ex(const char* file, const char* func, const int& line,
            std::exception* cause) {
        this->_cause = cause;
        this->_file = file;
        this->_func = func;
        this->_line = line;

        this->_msg = nullptr;
        this->_what = nullptr;
    }
    virtual ~SSFI_Ex() {
        if (_cause != nullptr) {
            delete _cause;
        }
    }
    void err() const noexcept {
        print(stderr);
    }
    void print(FILE* stream) const noexcept {
        int ret = 0;
        std::string str;
        fprintf(stream, "%s(%s:%d) %s: ", _func, _file, _line,
                typeid(this).name());
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
                fprintf(stream, "%s: %s\n", typeid(_cause).name(),
                        _cause->what());
            }
        } else {
            fprintf(stream, "\n");
        }
    }
    bool hasCause() const noexcept {
        return (_cause != nullptr);
    }
    const char * what() const noexcept {
        if (_what != nullptr) {
            return _what;
        }
        if (_cause != nullptr) {
            return _cause->what();
        }
        return "unknown";
    }
};

#endif /* ERR_H_ */
