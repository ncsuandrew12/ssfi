#include "err.h"

#include <stdarg.h>

#include <memory>
#include <string>
#include <typeinfo>
#include <thread>

#include "util.h"

SSFI_Ex::SSFI_Ex(const char* file, const int& line, const char* func,
        const char* w, const char* format, ...) :
        _cause(nullptr), _file(file), _func(func), _line(line), _what(w) {
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

SSFI_Ex::SSFI_Ex(const char* file, const int& line, const char* func,
        std::exception* cause, const char* w, const char* format, ...) :
        _cause(cause), _file(file), _func(func), _line(line), _what(w) {
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

SSFI_Ex::~SSFI_Ex() {
    if (_cause != nullptr) {
        delete _cause;
    }
}

void SSFI_Ex::err(const char* file, const int& line, const char* func) const {
    print(stderr, file, line, func);
}

std::string SSFI_Ex::msg() const {
    std::string ret;
    ret.append(printf_to_string("%s: ", typeid(this).name()));
    ret.append(summary());
    ret.append("\n\t\tat ");
    ret.append(printf_to_string(LOC_PRINTF, _file, _line, _func));
    if (_cause != nullptr) {
        ret.append("\n\tcaused by: ");
        if (dynamic_cast<SSFI_Ex*>(_cause)) {
            ret.append(dynamic_cast<SSFI_Ex*>(_cause)->msg());
        } else {
            ret.append(
                    printf_to_string("%s: %s", typeid(_cause).name(),
                            _cause->what()));
        }
    }
    return ret;
}

void SSFI_Ex::print(FILE* f, const char* file, const int& line,
        const char* func) const {
#ifdef SSFI_DEBUG
    fprintf(f, "T%d %s:%d(%s) %s\n", std::this_thread::get_id(), file, line,
            func, msg().c_str());
#else
    fprintf(f, "%s\n", summary().c_str());
#endif
}

std::string SSFI_Ex::summary() const {
    if (!_msg.empty()) {
        return _msg;
    } else if (_what != nullptr) {
        return std::string(_what);
    }
    return "Unknown error occurred.";
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
