#include <stdarg.h>

#include <memory>
#include <string>
#include <typeinfo>
#include <thread>

#include "ssfi_ex.h"
#include "util.h"

Ssfi_Ex::Ssfi_Ex(const char* file, const int& line, const char* func,
        const char* w, const char* format, ...) :
        _file(file), _func(func), _line(line), _what(w) {
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

void Ssfi_Ex::err(const char* file, const int& line, const char* func) const {
    print(stderr, file, line, func);
}

std::string Ssfi_Ex::msg() const {
    std::string ret;
    ret.append(printf_to_string("%s: ", typeid(this).name()));
    ret.append(summary());
    ret.append("\n\t\tat ");
    ret.append(printf_to_string(LOC_PRINTF, _file, _line, _func));
    return ret;
}

void Ssfi_Ex::print(FILE* f, const char* file, const int& line,
        const char* func) const {
#ifdef SSFI_DEBUG
    fprintf(f, "T%d %s:%d(%s) %s\n", std::this_thread::get_id(), file, line,
            func, msg().c_str());
#else
    fprintf(f, "Error: %s\n", summary().c_str());
#endif
}

std::string Ssfi_Ex::summary() const {
    if (!_msg.empty()) {
        return _msg;
    }
    return std::string(what());
}

const char* Ssfi_Ex::what() const noexcept {
    if (_what != nullptr) {
        return _what;
    }
    return "Unknown error occurred.";
}
