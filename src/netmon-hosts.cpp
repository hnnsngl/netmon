#include "netmon-hosts.hpp"

#include <ifstream>
#include <string>

/// derive the host group list from the host name list
HostgroupList NetmonHosts::createHostgroupList( const HostnameList & hostnames ) const
{
	HostgroupList groups;
	for( auto it : hostnames )
		groups.push_back(it->second);

	return groups;
}

/// read list of hosts from a netmon-hosts file
HostnameList NetmonHosts::createHostNameTree( const std::string & filename ) const
{
	// simple host file  syntax:
	// - comments start with a hash # symbol and continue the rest of the line
	// - a hostgroup is declared via brackets "[My favourite hosts]" followed by hosts
	// - a default hostgroup is declared implicitly
	// - declared hosts are added to the last hostgroup declared
	// - hosts may be declared in multiple hostgroups
	// - a host may be hostname ("eucken")  or a netgroup ("@workstation-all")
	// - host and netgroup declarations must be separated by whitespace

	HostnameList hostnames;
	
	std::ifstream if( filename );
	std::string input;
	while( getline(if, input) ){
		
	}
}
