#ifndef LOG_H_
#define LOG_H_

#ifdef SSFI_DEBUG

#include <stdio.h>

#include "util.h"

extern int flog(FILE* stream, const char* file, const int& line,
        const char* func, const char* format, va_list args);

extern int flogv(FILE* stream, const char* file, const int& line,
        const char* func, const char* format, ...);

extern void log(const char* file, const int& line, const char* func,
        const char* format, ...);

#else

#define log(...)

#endif // SSFI_DEBUG

#endif /* LOG_H_ */
