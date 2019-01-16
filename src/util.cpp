#include "util.h"

#include <stdarg.h>
#include <stdio.h>

#include <memory>

#include "log.h"

std::string printf_to_string(const char* format, ...) {
    std::string ret;

    va_list args;
    va_start(args, format);
    auto buf = std::unique_ptr<char[]>(new char[1]);
    size_t len = vsnprintf(buf.get(), 1, format, args);
    va_end(args);

    buf = std::unique_ptr<char[]>(new char[len + 1]);

    va_start(args, format);
    vsnprintf(buf.get(), len + 1, format, args);
    va_end(args);

    return {buf.get(), len};
}
