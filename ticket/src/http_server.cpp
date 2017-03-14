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

void err_exit(const char* txt) {
	printf("error: %s", txt);
	exit(1);
}

extern void parse_http_req(const char* req, int size, req_line* out);


// Starts a tcp server, listening for requests.
// Incoming requests are passed to the callback function.
void shttp_start(int port, void (*callback)(req_line*, void*, int, int)) {
	printf("starting httpserver at port: %d\n", port);

	static char buffer[256];
	int sock_fd, client_fd, err, length;
	struct sockaddr_in my_addr, client_addr;
	fd_set input_fdset, read_fdset;
	socklen_t size;

	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		err_exit("could not create socket");
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	err = bind(sock_fd, (const sockaddr*) &my_addr, sizeof(struct sockaddr_in));
	if (err == -1) 
		err_exit("could not bind to port");

	err = listen(sock_fd, 1);
	if (err == -1) 
		err_exit("could not listen");

	printf("sock_fd fd: %d\n", sock_fd);

	FD_ZERO(&input_fdset);
	FD_SET(sock_fd, &input_fdset);

	while(1) {
		read_fdset = input_fdset;
		// block until input arrives
		printf("blocking for select...\n");
		if (select(FD_SETSIZE, &read_fdset, NULL, NULL, NULL) == -1)
			err_exit("server select failed");

		printf("select returned\n");

		for (int i = 0; i < FD_SETSIZE; ++i) {
			if (FD_ISSET(i, &read_fdset)) {
				printf("read ready for fd: %d\n", i);
				if (i == sock_fd) {
					// Connection request on our server socket
					printf("new connection request\n");
					int newc;
					size = sizeof(client_addr);
					newc = accept(sock_fd, (struct sockaddr*) &client_addr, &size);
					if (newc < 0) 
						err_exit("accept failed");

					printf("Server: client connected %s, port %hu.\n",
										inet_ntoa(client_addr.sin_addr),
										ntohs(client_addr.sin_port));
					FD_SET(newc, &input_fdset);
				}
				else {
					// Data arriving on connected socket
					printf("receiving data from connected socket\n");
					char buffer[512];
					char outbuf[512];
					sprintf(outbuf, "hello from server\r\n");
					int l = read(i, buffer, 512);
					if (l <= 0) {
						printf("ups, nothing coming! client seems to have disconnected!\n");
						FD_CLR(i, &input_fdset);
						break; // only break from for loop
					}

					// We are still good, client did not actually disconnect, 
					// so lets continue to read in what it has sent us:
					for (int c = 0; c < l; c++) {
						int ch = buffer[c];
						//if (ch == '\n') 
							//printf("newline detected\n");
						//else if (ch == '\r')
							//printf("carr. return detected\n");
						//else 
							//printf("standard char: %c\n", ch);
					}
					buffer[l] = '\0';
					printf("received %d bytes: %s", l, buffer);
					//write(i, outbuf, 20);
					
					// do some http parsing here
					req_line rl;
					// this alloc is much too big/too small 
					// but I just don't know how to alloc better for now.
					rl.method = (char*) malloc(512);
					rl.uri = (char*) malloc(512);
					rl.http_version = (char*) malloc(100);
					memset(rl.method, 0, 512);
					memset(rl.uri, 0, 512);
					memset(rl.http_version, 0, 100);

					parse_http_req(buffer, l, &rl);							
					printf("rl.method: %s\n", rl.method);
					printf("rl.method uri: %s\n", rl.uri);
					printf("rl.method http version: %s\n", rl.http_version);
					callback(&rl, buffer, 512, i);
					free(rl.method);
					free(rl.uri);
					free(rl.http_version);
				}
			}
		}

		printf("after check loop\n");	

	}

	printf("out of main loop\n");
	close(client_fd);
	close(sock_fd);


}
