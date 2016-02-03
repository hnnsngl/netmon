#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <exception>

struct NetGroupException : public std::exception
{
	virtual const char *what() const throw() { return "Failed to initiate netGroup lookup"; }
};

struct NetGroupTriple
{
	std::string hostname, username, domainname;
	explicit NetGroupTriple( char* _host, char* _user, char* _domain )
	{
		if( _host != 0 ) hostname = _host;
		if( _user != 0 ) username = _user;
		if( _domain != 0 ) domainname = _domain;
	}
};

std::vector<NetGroupTriple> getNetGroupTriples(std::string netgroup);

struct NetGroup
{
	typedef std::vector<NetGroupTriple>::const_iterator iterator;

	const std::string name;
	const std::vector<NetGroupTriple> triples;

	/// construct NetGroup and perform name service lookup
	NetGroup(std::string name) : name(name), triples(getNetGroupTriples(name)) {}

	/// read only access to netgroup triples
	const NetGroupTriple& operator[](int i) { return triples[i]; }
	/// number of triples
	size_t size() { return triples.size(); }

	iterator begin() const {return triples.cbegin();}
	iterator end() const {return triples.cend();}
	iterator cbegin() const {return triples.cbegin();}
	iterator cend() const {return triples.cend();}
};
