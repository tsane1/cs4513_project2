/**
 * dsh_client.c
 * Command-line client for CS 4513 Project 2
 *
 * @author 	Tanuj Sane
 * @since 	3/26/2018
 * @verson	1.0
 *
 * Changelog:
 * - 1.0 Initial commit
 */

#include "dsh_shared.h"
#include "util.h"

/**
 * Set up the client and start the authorization process
 *
 * @return The descriptor of the opened socket
 */
int setup_client(char* ip_address) {
	int descr; struct sockaddr_in server; unsigned int server_len;
	
	/* Open a socket for this server */
	descr = try(socket(PF_INET, SOCK_STREAM, 0));
	
	/* Create the server information */
	memset(&server, 0, sizeof(server));					// Clear server struct
	server.sin_family = AF_INET;						// Internet / IP
	server.sin_addr.s_addr = inet_addr(ip_address);		// Accept from any address
	server.sin_port = htons(DSH_PORT);					// Listen on the predefined dsh port
	
	/* Connect to the server */
	server_len = sizeof(server);
	try(connect(descr, (struct sockaddr*) &server, server_len));
	
	/* If you got here, the server is up and running */
	info("dsh client activated!");
	
	return descr;
}

int main() {
	int socket_descr = setup_client("127.0.0.1");
	
	char buffer[BUFF_SIZE];
	strcpy(buffer, "HELLO FROM CLIENT!");
	try(send(socket_descr, buffer, BUFF_SIZE, 0));
	
	return EXIT_SUCCESS;
}