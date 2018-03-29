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
	printf("----------------- CS 4513 Project 2 -----------------\n");
	printf("Usage: %s [-p port] [-d dir] [-h]\n", my_name);
	printf("  -p Specify a port to serve on (default is 4513)\n");
	printf("  -d Specify a directory to serve from\n");
	printf("  -h Display this help and exit\n");
	printf("-----------------------------------------------------\n");
	exit(EXIT_FAILURE);
}

/**
 * Set up the server and start listening for requests
 *
 * @return The descriptor of the opened socket
 */
int setup_server(long port) {
	int descr; struct sockaddr_in server; unsigned int server_len;
	
	/* Open a socket for this server */
	descr = try(socket(PF_INET, SOCK_STREAM, 0));
	
	/* Create the server information */
	server.sin_family = AF_INET;					// Internet / IP
	server.sin_addr.s_addr = htonl(INADDR_ANY);		// Accept from any address
	server.sin_port = htons(port);					// Listen on the given port
	
	/* Bind the server information to the socket */
	server_len = sizeof(server);
	try(bind(descr, (struct sockaddr*) &server, server_len));
	try(listen(descr, MAX_CONNECTIONS));
	
	/* If you got here, the server is up and running */
	info("dsh server listening on %d", port);
	
	return descr;
}

/**
 * Dispatch handling of a specific connection to a child process
 */
void dispatch() {

}

/** MAIN */
int main(int argc, char** argv) {
	srand(time(NULL));
	
	// Set up configuration from commandline flags
	opterr = 0; char opt; long port = DSH_PORT; char* directory = getcwd(NULL, 0);
	while((opt = getopt(argc, argv, "hp:d:")) != -1) {
		switch(opt) {
			case 'h': help(argv[0]); break;
			case 'p': port = strtol(optarg, NULL, 10); break;
			case 'd': directory = optarg; break;
			case '?': 
				if(optopt == 'p') err("Must specify a port with -p");
				else if(optopt == 'd') err("Must specify a port with -d");
				else warn("Ignoring unrecognized flag -%c", optopt);
				break;
		}
	}
	int socket_descr = setup_server(port);
	
	struct sockaddr_in incoming; int incoming_len = sizeof(incoming);
	char buffer[BUFF_SIZE];
	
	#define EVER	;;
	for(EVER) {
		// Spin waiting for connections to the server
		int accepted_socket = try(accept(socket_descr, (struct sockaddr*) &incoming, (socklen_t*) &incoming_len));
		
		// When a connection comes in, go through the auth process
		try(read(accepted_socket, buffer, BUFF_SIZE));
		info("%s wants to connect, authorizing...", buffer);
		
		int random_number = rand() % 100; int sendable = htonl(random_number);
		dbg("Sent %d -> %d", random_number, sendable);
		try(send(accepted_socket, &sendable, sizeof(sendable), 0));
	}
	
	close(socket_descr);
	return EXIT_SUCCESS;
}