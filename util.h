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

#define dbg(msg)	log_base(0, msg, __FILE__, __LINE__)
#define info(msg)	log_base(1, msg, __FILE__, __LINE__)
#define warn(msg)	log_base(2, msg, __FILE__, __LINE__)
#define err(msg)	log_base(3, msg, __FILE__, __LINE__)
void log_base(int level, char* message, char* file, int line);

#define try(retval)	try_base(retval, __FILE__, __LINE__)
int try_base(int retval, char* file, int line);

#endif	/* UTIL_H */