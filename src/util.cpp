#include "util.h"

#include <memory>
#include <stdarg.h>
#include <stdio.h>

#include "log.h"

int printf_len(const size_t& size, const char* format, va_list args) {
    auto buf = std::unique_ptr<char[]>(new char[size]);
    return vsnprintf(buf.get(), size, format, args);
}

int printf_len(const char* format, va_list args) {
    return printf_len(1, format, args);
}

int printf_len(const char* format, ...) {
    int ret;

    va_list args;
    va_start(args, format);
    ret = printf_len(1, format, args);
    va_end(args);

    return ret;
}

std::string printf_to_string(const char* format, ...) {
    int rc = 0;
    std::string ret;

    va_list args;
    va_start(args, format);
    size_t len = printf_len(format, args);
    va_end(args);

    auto buf = std::unique_ptr<char[]>(new char[len + 1]);

    va_start(args, format);
    rc = vsnprintf(buf.get(), len + 1, format, args);
    va_end(args);

    return {buf.get(), len};
}

std::string printf_to_string(const size_t& size, const char* format,
        va_list args) {
    auto buf = std::unique_ptr<char[]>(new char[size + 1]);
    int rc = 0;

    rc = vsnprintf(buf.get(), size + 1, format, args);

    if (rc < 0) {
        fprintf(stderr,
                "%s:%d(%s) Failed to print to string (len=%d). Format string: %s.\n",
                LOC, size, format);
        return "";
    }

    size_t len = rc;
    if (len < size) {
        printf("%s:%d(%s) string (%d) was truncated by %d\n", LOC, size,
                size - len);
    }

    return {buf.get(), len};
}
