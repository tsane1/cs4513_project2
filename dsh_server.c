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

char* getPassword(char* givenUsername, Account accounts[] ){
	for (int i=0; i< 5; i++){
		if (strncmp(accounts[i]->username, givenUsername, 100)== 0){
			return accounts[i]->password;
		}
	}
	err("Username not found\n");
	return 0;
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
void run_command(char* directory, char* command, char* output) {
	FILE *file;
	char buf[100];

	file = popen(command,"r");
	if(file == NULL){
		 err("command failed to run");
	 }
	if (errno != 0){
		sprintf(output, "%s", strerror(errno));
		 dbg("GOT HERE \n");
	}else{

		while(fgets(buf,sizeof(buf)-1,file)!= NULL){
			strcat(output,buf);
		}
	}
	dbg("FILE: %s \n", output);
	pclose(file);

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
	pid_t pid;
	int socket_descr = setup_server(port);
	struct sockaddr_in incoming; int incoming_len = sizeof(incoming);
	char buffer[BUFF_SIZE];
	int i = 0;
	Account accounts[5];
	for (i=0; i<5; i++){
		Account a = malloc(sizeof (struct account));
		accounts[i]=a;
	}
	accounts[0]->username = "Foo";
	accounts[0]->password = "Bar";

	accounts[1]->username = "Drew";
	accounts[1]->password = "GET";

	accounts[2]->username = "Toby";
	accounts[2]->password = "4321";

	accounts[3]->username = "Tanuj";
	accounts[3]->password = "Hair";

	accounts[4]->username = "grader";
	accounts[4]->password = "Admin";



	#define EVER	;;
	for(EVER) {
		// Spin waiting for connections to the server

		int accepted_socket = try(accept(socket_descr, (struct sockaddr*) &incoming, (socklen_t*) &incoming_len));

		pid = fork();
		if(pid == 0){

			// When a connection comes in, go through the auth process
			char* givenUsername = malloc(sizeof(char)*100);
			try(read(accepted_socket, givenUsername, BUFF_SIZE));
			info("%s wants to connect, authorizing...", buffer);
			char* cmd = malloc(sizeof(char)*100);

			try(recv(accepted_socket, cmd, sizeof(char)*100, 0));
			int random_number = rand() % 90 + 10; //must be 10-99
			int sendable = htonl(random_number);
			dbg("Sent %d -> %d", random_number, sendable);
			try(send(accepted_socket, &sendable, sizeof(sendable),  MSG_CONFIRM));
		  printf("stuff\n" );
			char* recvable = malloc(sizeof(char)*1000);
			try(recv(accepted_socket, recvable, sizeof(char)*100, MSG_WAITALL));

			double preValidationTime = get_time();
			char* randomChar = (char*)malloc(sizeof(char)*10);
			sprintf(randomChar, "%d", random_number);
			char* encrypted = crypt(getPassword(givenUsername, accounts),randomChar);
			double postValidationTime = get_time();
			dbg("%s, %s -> %s", getPassword(givenUsername, accounts),randomChar, encrypted);
			dbg("Server Recieved: %s, compare to %s", recvable, encrypted);
			char testData[1001];
			for (int i = 0; i< 1000; i++){
				testData[i]='a';
			}
			double startThroughTime = get_time();
			try(send(accepted_socket,testData, sizeof(char)*1001,  MSG_CONFIRM));
			double endThroughTime = get_time();
			printf("Throughput: %f\n", 1001/ (endThroughTime - startThroughTime) );
			printf("Authorization Time: %f\n",postValidationTime -preValidationTime );


			if(strcmp(recvable, encrypted) == 0){
				char output[2000];
				run_command(directory, cmd, output);
				dbg(output);
				try(send(accepted_socket, output, sizeof(output), MSG_CONFIRM));
			}else{
				dbg("Incorrect Password\n");
			}
			_exit(3);
		}
	}

	close(socket_descr);
	return EXIT_SUCCESS;
}
