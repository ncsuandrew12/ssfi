#include "log.h"

#include <stdarg.h>

#include <iostream>
#include <string>

int log_inner(FILE* stream, const char* file, const char* func,
        const int& line, const char* format, ...) {
    va_list args;
    int ret = 0;
    std::string str;
    str.append(file);
    str.append(":");
    str.append(std::to_string(line));
    str.append("(");
    str.append(func);
    str.append(") ");
    str.append(format);

    va_start (args, format);
    ret = vfprintf(stream, str.c_str(), args);
    va_end (args);
    return ret;
}
