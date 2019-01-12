/*
 * log.h
 *
 *  Created on: Jan 13, 2019
 *      Author: afelsher
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#ifdef SSFI_DEBUG
#define ERR(f, ...) \
    log_inner(stderr, __FILE__, __FUNCTION__, __LINE__, f, __VA_ARGS__)
#define LOG(f, ...) \
    log_inner(stdout, __FILE__, __FUNCTION__, __LINE__, f, __VA_ARGS__)
#else
#define ERR(f, ...)
#define LOG(f, ...)
#endif

int log_inner(FILE*, const char*, const char*, const int&, const char*, ...);

#endif /* LOG_H_ */
