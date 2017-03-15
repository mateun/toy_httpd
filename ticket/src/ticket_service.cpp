#include <stdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "http_types.h"

// This is the callback which handles incoming requests.
void my_callback(req_line* req_line, void* data, int data_size, int client_nr) {
	printf("msg in callback: %s, %s, %d\n", req_line->method, req_line->uri, client_nr);

	// Send response to the client, 
	// as the client_nr is in fact a file descriptor 
	// to which you can directly write to the client. 
	write(client_nr, "hello from the server!\r\n", 26); 
}

extern void shttp_start(int port, void (*callback)(req_line*, void*, int, int)); 


int main(int argc, char** args) {
	int port = 8890;	
	if (argc == 2) {
		port = strtol(args[1], NULL, 10);	
		printf("port set to %d\n", port);
	}
	shttp_start(port, my_callback);
	return 0;
	
}
