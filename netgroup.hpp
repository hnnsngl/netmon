#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <exception>

#include <netdb.h>

typedef std::tuple< std::string, std::string, std::string > netGroupTriple;

class netGroupException : public std::exception {
	virtual const char* what() const throw(){
		return "Failed to initiate netGroup lookup." ;
	}
};

std::vector<netGroupTriple>
getNetGroup( std::string netgroup )
{
	const char* empty = "";
	std::vector<netGroupTriple> entries;
	char *hostname, *username, *domainname;
	if( setnetgrent( netgroup.c_str() ) != 1 )
		throw netGroupException();

	while( getnetgrent( &hostname, &username, &domainname ) == 1 )
		entries.push_back( std::make_tuple( (hostname != 0) ? hostname : empty,
		                                    (hostname != 0) ? hostname : empty,
		                                    (hostname != 0) ? hostname : empty ) );

	endnetgrent();

	return entries;
}
