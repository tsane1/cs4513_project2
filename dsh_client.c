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

/** Print the help and exit */
void help(char* my_name) {
	printf("------------------------ dsh_client ------------------------\n");
	printf("Usage: %s {-c command} {-s hostname} [-u username] [-h]\n", my_name);
	printf("  -c The command in quotes to run\n");
	printf("  -s The host the server is running on\n");
	printf("  -u Specify a username to log in with (default is 'grader')\n");
	printf("  -h Display this help and exit\n");
	printf("------------------------------------------------------------\n");
	exit(EXIT_FAILURE);
}

/**
 * Set up the client and start the authorization process
 *
 * @return The descriptor of the opened socket
 */
int setup_client(char* host) {
	int descr; struct sockaddr_in server; unsigned int server_len;
	
	/* Open a socket for this server */
	descr = try(socket(PF_INET, SOCK_STREAM, 0));
	
	/* Create the server information */
	server.sin_family = AF_INET;						// Internet / IP
	server.sin_addr.s_addr = inet_addr(host);			// Connect to specified address
	server.sin_port = htons(DSH_PORT);					// Listen on the predefined dsh port
	
	/* Connect to the server */
	server_len = sizeof(server);
	try(connect(descr, (struct sockaddr*) &server, server_len));
	
	info("Connected to server on %s", host);
	
	return descr;
}

int main(int argc, char** argv) {
	// Set up configuration from commandline flags
	opterr = 0; char opt; char* cmd = NULL; char* host = NULL; char* username = DSH_DEFAULT_USERNAME;
	while((opt = getopt(argc, argv, "hc:s:u:")) != -1) {
		switch(opt) {
			case 'h': help(argv[0]); break;
			case 'c': cmd = optarg; break;
			case 's': host = optarg; break;
			case 'u': username = optarg; break;
			case '?': 
				if(optopt == 'c') err("Must specify a command with -c");
				else if(optopt == 's') err("Must specify a host with -s");
				else if(optopt == 'u') warn("Must specify a username with -u");
				else warn("Ignoring unrecognized flag -%c", optopt);
				break;
		}
	}
	
	if(cmd == NULL || host == NULL) err("Must specify both -c and -s options, use -h for help");
	int socket_descr = setup_client(host);
	
	// Go through the authorization process
	try(send(socket_descr, username, sizeof(username), 0));
	char* password = getpass("Password: ");
	
	int random_number; 
	try(recv(socket_descr, &random_number, sizeof(int), MSG_WAITALL));
	
	char* encrypted = crypt(password, &random_number);
	err(strerror(errno));
	dbg("%s", encrypted);
	try(send(socket_descr, encrypted, sizeof(encrypted), 0));
	
	close(socket_descr);
	return EXIT_SUCCESS;
}