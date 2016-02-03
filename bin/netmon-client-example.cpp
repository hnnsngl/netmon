#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>					// POSIX sockets
#include <netinet/in.h>
#include <arpa/inet.h>					// numeric IP address manipulations
#include <netdb.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << "netmon-client host port" << std::endl;
		exit(0);
	}

	// resolve hostname
	char *hostname = argv[1];
	char *service = argv[2];
	addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	if (getaddrinfo(hostname, service, &hints, &res) != 0) {
		std::cerr << "cannot resolve host" << std::endl;
		exit(0);
	}

	int status = 0;
	int sockd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockd == -1) {
		std::cerr << "cannot create socket" << std::endl;
		exit(-1);
	}

	const int buffer_size = 1024;
	char buffer[buffer_size];

	/* connect to the server */
	// status = connect(sockd, // (struct sockaddr*)
	//                  &serv_name, sizeof(serv_name));
	status = connect(sockd, res->ai_addr, res->ai_addrlen);
	if (status == -1) {
		perror("Connection error");
		std::cerr << "connection refused?" << std::endl;
		exit(1);
	}

	memset(&buffer, 0, buffer_size);
	while (read(sockd, buffer, buffer_size)) {
		std::cout << buffer;
		memset(&buffer, 0, buffer_size);
	}
	std::cerr << std::endl << "connection closed" << std::endl;

	freeaddrinfo(res);
	close(sockd);
}
