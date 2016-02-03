#include <iostream>
#include <vector>
#include <netdb.h>
#include "netgroup.hpp"

void getnetgroup_c(int argc, char **argv)
{
	// search all supplied netgroups ..
	std::cout << argv[1] << std::endl;

	char *hostname, *username, *domainname;
	if (setnetgrent(argv[1]) != 1) {
		std::cerr << "ERROR: setnetgrent" << std::endl;
	}
	while (getnetgrent(&hostname, &username, &domainname) == 1) {
		std::cout << "(";
		if (hostname != 0)
			std::cout << hostname;
		std::cout << ", ";
		if (username != 0)
			std::cout << username;
		std::cout << ", ";
		if (domainname != 0)
			std::cout << domainname;
		std::cout << ")" << std::endl;
	}
	endnetgrent();
}

int main(int argc, char **argv)
{
	std::cout << "Using C API" << std::endl;
	getnetgroup_c(argc, argv);

	std::cout << "Using netgroup.hpp" << std::endl;
	for ( auto triple : NetGroup(std::string(argv[1])))
		std::cout << triple.hostname << std::endl;
}
