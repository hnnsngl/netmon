#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <exception>

#include <netdb.h>

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

struct NetGroup
{
	typedef std::vector<NetGroupTriple> NetGroupTriples;
	typedef NetGroupTriples::const_iterator iterator;

	const std::string name;
	const NetGroupTriples triples;

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

      private:
	NetGroupTriples getNetGroupTriples(std::string netgroup)
	{
		std::vector<NetGroupTriple> triples;

		try {
			if (setnetgrent(netgroup.c_str()) != 1)
				throw NetGroupException();

			char *hostname, *username, *domainname;
			while (getnetgrent(&hostname, &username, &domainname) == 1) {
				triples.push_back(NetGroupTriple(hostname, username, domainname));
			}
		} catch (NetGroupException e) {
			std::cerr << e.what() << " ( for " << netgroup << " )" << std::endl;
		}

		endnetgrent();
		return triples;
	}
};
