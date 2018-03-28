/**
 * dsh_server.c
 * Server code for CS 4513 Project 2
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

/** Print the help and exit */
void help(char* my_name) {
	printf("--------------- CS 4513 Project 2 ---------------\n");
	printf("Usage: %s [-h]\n", my_name);
	printf("  -h Display this help and exit\n");
	printf("-------------------------------------------------\n");
	exit(EXIT_FAILURE);
}

/**
 * Set up the server and start listening for requests
 *
 * @return The descriptor of the opened socket
 */
int setup_server() {
	int descr; struct sockaddr_in server; unsigned int server_len;
	
	/* Open a socket for this server */
	descr = try(socket(PF_INET, SOCK_STREAM, 0));
	
	/* Create the server information */
	server.sin_family = AF_INET;					// Internet / IP
	server.sin_addr.s_addr = htonl(INADDR_ANY);		// Accept from any address
	server.sin_port = htons(DSH_PORT);				// Listen on the predefined dsh port
	
	/* Bind the server information to the socket */
	server_len = sizeof(server);
	try(bind(descr, (struct sockaddr*) &server, server_len));
	try(listen(descr, MAX_CONNECTIONS));
	
	/* If you got here, the server is up and running */
	info("dsh server activated!");
	
	return descr;
}

/** MAIN */
int main(int argc, char** argv) {
	// Set up configuration from commandline flags
	opterr = 0; char opt;
	while((opt = getopt(argc, argv, "h")) != -1) {
		switch(opt) {
			case 'h': help(argv[0]); break;
			case '?': warn("Ignoring unrecognized flag");
		}
	}
	
	int socket_descr = setup_server();
	
	struct sockaddr_in incoming; int incoming_len = sizeof(incoming);
	char buffer[BUFF_SIZE];
	
	#define EVER	;;
	for(EVER) {
		int accepted_socket = try(accept(socket_descr, (struct sockaddr*) &incoming, (socklen_t*) &incoming_len));
		try(read(accepted_socket, buffer, BUFF_SIZE));
		printf("Recieved %s from client!\n", buffer);
	}
	
	close(socket_descr);
	return EXIT_SUCCESS;
}