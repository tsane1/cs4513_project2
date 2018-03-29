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
void log_base(int level, char* file, int line, char* message, ...) {
	// Print the level
	fprintf(stderr, "[%s]: ", strlvl(level));
	
	// Print the message with variadic parameters
	va_list args; va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);
	
	// Print the file and line if this is a warning or error
	if(level > 1) fprintf(stderr, " (%s:%d)", file, line);
	fprintf(stderr, "\n");
	
	// Terinate on error
	if(level > 2) exit(EXIT_FAILURE);
}

/** The base try function for failing out properly */
int try_base(int retval, char* file, int line) {
	if(retval < 0) log_base(3, file, line, strerror(errno));
	else return retval;
}