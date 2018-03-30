/**
 * dsh_shared.h
 * Shared data and functions across the dsh server and client
 *
 * @author 	Tanuj Sane
 * @since 	3/26/2018
 * @verson	1.0
 *
 * Changelog:
 * - 1.0 Initial commit
 */

#ifndef DSH_SHARED_H
#define DSH_SHARED_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <crypt.h>
#define _XOPEN_SOURCE
#include <unistd.h>

#define DSH_PORT				4513
#define MAX_CONNECTIONS			10
#define BUFF_SIZE				256
#define DSH_DEFAULT_USERNAME	"grader"

#endif
