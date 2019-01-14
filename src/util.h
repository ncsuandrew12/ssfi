#ifndef UTIL_H_
#define UTIL_H_

#include <memory>
#include <string>

int printf_len(const size_t& size, const char* format, va_list args);

extern int printf_len(const char* format, va_list args);

extern int printf_len(const char* format, ...);

extern std::string printf_to_string(const char* format, ...);

extern std::string printf_to_string(const size_t& size, const char* format,
        va_list args);

#define LOC __FILE__, __LINE__, __FUNCTION__
#define LOC_PRINTF "%s:%d(%s) "

#endif /* UTIL_H_ */
