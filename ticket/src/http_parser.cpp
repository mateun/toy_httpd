#include <stdio.h>
#include <string.h>
#include "http_types.h"



void mystrcpy(char *dest, int size, const char* src) {
	for (int i = 0; i < size; i++) {
		char c = src[i];
		dest[i] = c;
		if (c == '\0') 
			break;
	}
}

void parse_http_req(const char* req, int size, req_line* req_info) {
	printf("parsing request\n");
	char lineBuffer[512];

	// just parse the first line...
	for (int i=0; i < size; ++i) {
		char c = req[i];
		if (c == '\n') {
			printf("line end detected, breaking parsing!\n");
			lineBuffer[i] = '\0';
			printf("line: %s\n", lineBuffer);
			break;
		}
		else {
			lineBuffer[i] = c;
		}
	}

	// now split the first line into 
	// METDHOD URI HTTP-VERSION
	int i = 0;
	int methodCounter = 0;
	int uriCounter = 0;
	int httpVCounter = 0;
	char methodBuf[50];
	char uriBuf[500];
	char httpVersionBuf[50];
	int state = 0; // 0=method, 1=uri, 2=httpversion
	char c = ' ';
	while ((c = lineBuffer[i++]) != 0) {
		// we are parsing the method
		if (state == 0) {
			if (c == ' ') {
				state = 1;
				methodBuf[i] = '\0';	
				continue;
			}
			else {
				methodBuf[methodCounter++] = c;
			}
		}

		else if (state == 1) {
			if (c == ' ') {
				state = 2;
				uriBuf[uriCounter] = '\0';	
				continue;
			}
			else {
				uriBuf[uriCounter++] = c;
			}

		}
		else if (state == 2) {
			if (c == ' ') {
				httpVersionBuf[httpVCounter] = '\0';	
				break;
			}
			else {
				httpVersionBuf[httpVCounter++] = c;
			}
		}
		
	}

	printf("method: %s\n", methodBuf);
	printf("uri: %s\n", uriBuf);
	printf("http version: %s\n", httpVersionBuf);
	
	mystrcpy(req_info->method, methodCounter, methodBuf);
	mystrcpy(req_info->uri, uriCounter, uriBuf);
	mystrcpy(req_info->http_version, httpVCounter, httpVersionBuf);
		
	
	printf("after strcpy: %s %s %s\n", req_info->method, req_info->uri, req_info->http_version);
	
}
