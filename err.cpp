#include "err.h"

#include <stdarg.h>
#include <stdio.h>

//#include <iostream>
#include <string>

std::string print_to_string(const size_t& max, const char* format,
        va_list args) {
    auto vbuf = std::unique_ptr<char[]>(new char[max + 1]);
    int rc = 0;

    rc = vsnprintf(vbuf.get(), max + 1, format, args);

    if (rc < 0) {
        fprintf(stderr,
                "%s(%s:%d) Failed to print to string (max=%d). Format string: %s.\n",
                __FUNCTION__, __FILE__, __LINE__, max, format);
        return "";
    }

    size_t s = rc;
    if (s < max) {
        return {vbuf.get(), s};
    }

    return {vbuf.get(), max};

}
