

struct req_line {
	char* method;
	char* uri;
	char* http_version;

	~req_line() { 
		printf("in dtr. of req_line\n");
	}

};
