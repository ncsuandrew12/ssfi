#include "log.h"

#include <stdarg.h>

#include <iostream>
#include <string>
#include <thread>

#ifdef SSFI_DEBUG

int flogv(FILE* stream, const char* file, const int& line, const char* func,
        const char* format, ...) {
    va_list args;
    int ret = 0;
    va_start (args, format);
    ret = flog(stream, file, line, func, format, args);
    va_end (args);
    return ret;
}

void log(const char* file, const int& line, const char* func,
        const char* format, ...) {
    va_list args;
    va_start(args, format);
    flog(stdout, file, line, func, format, args);
    va_end(args);
}

#endif // SSFI_DEBUG

int flog(FILE* stream, const char* file,
        const int& line, const char* func, const char* format, va_list args) {
    std::string str;
    str.append(printf_to_string("T-%ld ", std::this_thread::get_id()));
    str.append(printf_to_string(LOC_PRINTF, file, line, func));
    str.append(format);
    str.append("\n");
    return vfprintf(stream, str.c_str(), args);
}

void log_err(const char* file, const int& line, const char* func,
        const std::exception& ex) {
    log_err(file, line, func, "%s thrown: %s", typeid(ex).name(), ex.what());
}

void log_err(const char* file, const int& line, const char* func,
        const char* format, ...) {
    va_list args;
    va_start(args, format);
    flog(stderr, file, line, func, format, args);
    va_end(args);
}
