# Makefile for CS 4513 Project 2

FLAGS = -g -Wall
LIBS = -lcrypt

default: dsh dsh_server

# UTILTIES
util.o : util.c
	$(CC) -c util.c -o util.o

# CLIENT
dsh : dsh_client.o util.o
	$(CC) dsh_client.o util.o -o dsh $(LIBS)

dsh_client.o : dsh_client.c
	$(CC) $(FLAGS) -c dsh_client.c -o dsh_client.o

# SERVER
dsh_server : dsh_server.o util.o
	$(CC) dsh_server.o util.o -o dsh_server $(LIBS)

dsh_server.o : dsh_server.c
	$(CC) $(FLAGS) -c dsh_server.c -o dsh_server.o

# DOCS AND CLEANUP
docs:
	doxygen

clean:
	/bin/rm -f *.o dsh dsh_server
