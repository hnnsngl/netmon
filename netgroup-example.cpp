#include <iostream>
#include <vector>

#include <netdb.h>

int main( int argc, char** argv )
{
	// search all supplied netgroups ..
	std::cout << argv[1] << std::endl;

	char *hostname, *username, *domainname;
	if( setnetgrent( argv[1] ) != 1 ){
		std::cerr << "ERROR: setnetgrent" << std::endl;
	}
	int status = 0; while( getnetgrent( &hostname, &username, &domainname ) == 1 ){
		std::cout << "(";
		if(hostname != 0) std::cout << hostname;
		std::cout << ", ";
		if( username != 0 ) std::cout << username;
		std::cout << ", ";
		if( domainname != 0 ) std::cout << domainname != 0;
		std::cout << ")" << std::endl;
	}
	endnetgrent();
}
