#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>
#include <stdio.h>

#include "util.h"

#ifdef SSFI_DEBUG
extern int flogv(FILE* stream, const char* file, const int& line,
        const char* func, const char* format, ...);
extern int flog(FILE* stream, const char* file, const int& line,
        const char* func, const char* format, va_list args);

extern void log(const char* file, const int& line, const char* func,
        const char* format, ...);
#else
#define log(...)
#endif

extern void log_err(const char* file, const int& line, const char* func,
        const char* format, ...);

#endif /* LOG_H_ */
