/**
 * util.c
 * Implementations of useful utilities for CS 4513 Project 2
 *
 * @author  Tanuj Sane
 * @since	3/19/2018
 * @version 1.0
 *
 * Changelog:
 * - 1.0 Initial commit
 */

#include "util.h"

/** Convert a level integer to a string level representation */
char* strlvl(int level) {
	switch(level) {
		case 0: return "DEBUG"; break;
		case 1: return "INFO"; break;
		case 2: return "WARN"; break;
		case 3: return "ERROR"; break;
	}
}

/** The base logging function for printing messages */
void log_base(int level, char* message, char* file, int line) {
	if(level > 1) {
		fprintf(
			(level < 3) ? stdout : stderr,
			"[%s]: %s (%s:%d)\n",
			strlvl(level), message, file, line
		);
		if(level > 2) exit(EXIT_FAILURE);
	}
	else {
		printf("[%s]: %s\n", strlvl(level), message);
	}
	
}

/** The base try function for failing out properly */
int try_base(int retval, char* file, int line) {
	if(retval < 0) log_base(3, strerror(errno), file, line);
	else return retval;
}