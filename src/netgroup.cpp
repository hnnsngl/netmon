#include <netdb.h>
#include "netgroup.hpp"

std::vector<NetGroupTriple> getNetGroupTriples(std::string netgroup)
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
