#include "log.h"

#include <stdarg.h>

#include <iostream>
#include <string>

int flogv(FILE* stream, const char* file, const int& line, const char* func,
        const char* format, ...) {
    va_list args;
    int ret = 0;
    va_start (args, format);
    ret = flog(stream, file, line, func, format, args);
    va_end (args);
    return ret;
}

int flog(FILE* stream, const char* file,
        const int& line, const char* func, const char* format, va_list args) {
#ifdef SSFI_DEBUG
    std::string str;
    str.append(printf_to_string(LOC_PRINTF, file, line, func));
    str.append(format);
    return vfprintf(stream, str.c_str(), args);
#else
    return 0;
#endif
}

void log(const char* file, const int& line, const char* func,
        const char* format, ...) {
    va_list args;
    va_start(args, format);
    flog(stdout, file, line, func, format, args);
    va_end(args);
}

void log_err(const char* file, const int& line, const char* func,
        const char* format, ...) {
    va_list args;
    va_start(args, format);
    flog(stderr, file, line, func, format, args);
    va_end(args);
}
