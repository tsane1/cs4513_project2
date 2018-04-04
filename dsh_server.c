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
#include <time.h>
#include <string.h>
struct account{
	char* username;
	char* password;
};

#define Account struct account*

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

/**
 * executes a command and returns the output
 */
void run_command(char* directory, char* command) {
	dbg("starting command %s, %s",directory, command);
	pid_t pid;
	int p[2];
	char data[2000];

	if(pipe(p)) err("could not create pipe");
	pid = fork();
	if(pid == -1){
		err("fork failed");
	} else if(pid == 0){
		close(p[1]);
		dup2(p[1],1);
		close(p[0]);
		execl(directory,command, (char *) NULL);
		close(p[0]);
		int size = read(p[0],data,sizeof(data));
		printf("stdout is %s\n",data);
		err("command failed to exit %d", pid);
	} else {

		wait(NULL);
	}
	dbg("finished running");

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
	Account accounts = malloc(sizeof (struct account));
	accounts->username = "Foo";
	accounts->password = "Bar";
	#define EVER	;;
	for(EVER) {
		// Spin waiting for connections to the server
		int accepted_socket = try(accept(socket_descr, (struct sockaddr*) &incoming, (socklen_t*) &incoming_len));

		// When a connection comes in, go through the auth process
		char* givenUsername = malloc(sizeof(char)*100);
		try(read(accepted_socket, givenUsername, BUFF_SIZE));
		info("%s wants to connect, authorizing...", givenUsername);
		char* cmd = malloc(sizeof(char)*100);
		try(recv(accepted_socket, cmd, sizeof(char)*100, 0));

		dbg("%s CMD\n", cmd);
;
		int random_number = rand() % 90 +10; //must be 10-99
		int sendable = htonl(random_number);
		dbg("Sent %d -> %d", random_number, sendable);
		try(send(accepted_socket, &sendable, sizeof(sendable),  MSG_CONFIRM));
	  printf("stuff\n" );
		char* randomChar = (char*)malloc(sizeof(char)*10);
		sprintf(randomChar, "%d", random_number);
		char* encrypted = crypt(accounts->password,randomChar);
		dbg("%s, %s -> %s", accounts->password, randomChar, encrypted);
		char* recvable = malloc(sizeof(char)*1000);
		try(recv(accepted_socket, recvable, sizeof(char)*100, MSG_WAITALL));
		dbg("Server Recieved: %s, compare to %s", recvable, encrypted);
		if(strcmp(recvable, encrypted) == 0){
			run_command(directory, cmd);//TODO: Fix so cmd arguments are all processed and answer returend to client STDIN
		}else{
			printf("Incorrect Password\n");
		}
	}

	close(socket_descr);
	return EXIT_SUCCESS;
}
