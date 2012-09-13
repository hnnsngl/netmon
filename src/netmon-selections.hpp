#pragma once

#include <string>
#include <unordered_map>

// number of selections per hostname (because a host may be selected multiple times)
class	HostSelections
{
private:
	NetmonHosts & netmonHosts;
	std::unordered_map<std::string, int> items;

public:
	HostSelections( NetmonHosts& hosts )
		: netmonHosts(hosts)
	{}

	void selectHost( std::string hostname, bool enabled )
	{
		// check/sanitize hostname

		if( enabled )
			items[hostname]++ ;
		else
			items[hostname]-- ;
	}

	void selectGroup( std::string hostgroup, bool enabled )
	{
		for( const auto & hostname : netmonHosts.hostnames[hostgroup] )
			selectHost(hostname, enabled);
	}

	bool operator[]( std::string hostname )
	{
		return (items[hostname] > 0);
	}

	std::ostream& print( std::ostream& os ) const
	{
		for( const auto& item : items )
			os << item.first << "\t" << item.second << std::endl;
		return os;
	}
};
