#pragma once

#include <string>
#include <vector>
#include <exception>

#include <netdb.h>

struct NetGroupException : public std::exception {
	virtual const char* what() const throw(){
		return "Failed to initiate netGroup lookup" ;
	}
};

struct NetGroupTriple {
	std::string hostname, username, domainname;
	explicit NetGroupTriple( char* _host, char* _user, char* _domain )
	{
		if( _host != 0 ) hostname = _host;
		if( _user != 0 ) username = _user;
		if( _domain != 0 ) domainname = _domain;
	}
};

std::vector<NetGroupTriple> getNetGroupTriples( std::string netgroup )
{
	std::vector<NetGroupTriple> triples;

	try{
		if( setnetgrent( netgroup.c_str() ) != 1 )
			throw NetGroupException();

		char *hostname, *username, *domainname;
		while( getnetgrent( &hostname, &username, &domainname ) == 1 ){
			std::cerr<< "getnetgrent" << std::endl;

			triples.push_back(NetGroupTriple( hostname, username, domainname ));
		}
	}
	catch (NetGroupException e) {
		std::cerr << e.what() << " ( for " << netgroup << " )" << std::endl;
	}

	endnetgrent();
	return triples;
}
