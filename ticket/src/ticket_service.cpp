#include <stdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

void my_callback(const char* method, void* data, int data_size, int client_nr) {
	printf("msg in callback: %s, %d\n", method, client_nr);
}

extern void shttp_start(int port, void (*fn)(const char*, void*, int, int)); 


int main(int argc, char** args) {
	int port = 8890;	
	if (argc == 2) {
		port = strtol(args[1], NULL, 10);	
		printf("port set to %d\n", port);
	}
	shttp_start(port, my_callback);
	return 0;
	
}
