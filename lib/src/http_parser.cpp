#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "http_types.h"



void mystrcpy(char *dest, int size, const char* src) {
	for (int i = 0; i < size; i++) {
		char c = src[i];
		dest[i] = c;
		if (c == '\0') 
			break;
	}
}

struct Line {
	char* content = NULL;
	Line* next = NULL;

};

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
	memset(methodBuf, 0, 50);
	memset(uriBuf, 0, 500);
	memset(httpVersionBuf, 0, 50);
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

	// split into lines
	Line* first = new Line();
	
	// helper variables
	char* pch = NULL;
	char* input = (char*)req;
	char* cl = NULL;
	char* headerParts = NULL;
	int lineCounter = 0;	

	// move through all the lines
	// and store them in linked list.
	pch = strtok(input, "\r\n");
	first->content = (char*) malloc(512);
	strcpy(first->content, pch);


	Line* last = NULL;
	Line* second = NULL;
	while (pch != NULL) {
		printf("line: %s\n", pch);
		pch = strtok(NULL, "\r\n");
		lineCounter++;

		// We are after the last line, 
		// so we break out of the loop and
		// we copy the former first entry
		// to the last spot, so
		// we have a clean reversed list, where
		// the body content is first and the 
		// request line (with method, uri and http version)
		// is last.
		if (pch == NULL) {
			Line* firstcpy = new Line();
			firstcpy->content = (char*) malloc(512);
			strcpy(firstcpy->content, first->content);
			second->next = firstcpy;
			first = last;
					
			break;
		}
		
	 	{
			Line* l = new Line();
			l->content = (char*) malloc(512);
			strcpy(l->content, pch);
			Line* temp = first->next;
			first->next = l;
			l->next = temp;
			
			if (lineCounter == 1) 
				second = l;
			else 
				last = l;
		}	

	}

	Line* l = first;
	while (l != NULL) {
		printf("line content: %s\n", l->content);
		l = l->next;
	}

	
	
			
		
	// TODO: is there a content-length? what is the content-type?
	// create a buffer with the content-length. 
	// store the content in the buffer
	
}
