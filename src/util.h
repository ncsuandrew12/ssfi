#ifndef UTIL_H_
#define UTIL_H_

#include <string>

extern std::string printf_to_string(const char* format, ...);

#define LOC __FILE__, __LINE__, __FUNCTION__
#define LOC_PRINTF "%s:%d(%s) "

#endif /* UTIL_H_ */
