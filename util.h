/**
 * util.h
 * Definitions of useful utilities for CS 4513 Project 1
 *
 * @author  Tanuj Sane
 * @since	3/19/2018
 * @version 1.0
 *
 * Changelog:
 * - 1.0 Initial commit
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#define dbg(...)	log_base(0, __FILE__, __LINE__, __VA_ARGS__)
#define info(...)	log_base(1, __FILE__, __LINE__, __VA_ARGS__)
#define warn(...)	log_base(2, __FILE__, __LINE__, __VA_ARGS__)
#define err(...)	log_base(3, __FILE__, __LINE__, __VA_ARGS__)
void log_base(int level, char* file, int line, char* message, ...);

#define try(retval)	try_base(retval, __FILE__, __LINE__)
int try_base(int retval, char* file, int line);

#endif	/* UTIL_H */