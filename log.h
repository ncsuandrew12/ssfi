/*
 * log.h
 *
 *  Created on: Jan 13, 2019
 *      Author: afelsher
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>
#include <stdio.h>

extern int log_inner(FILE*, const char*, const char*, const int&, const char*, ...);

#ifdef SSFI_DEBUG
#define log(f, ...) \
    log_inner(stdout, __FILE__, __FUNCTION__, __LINE__, f, __VA_ARGS__)
#else
#define log(f, ...)
#endif

#endif /* LOG_H_ */
