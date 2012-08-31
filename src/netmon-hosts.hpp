#pragma once

#include <map>
#include <set>
#include <string>

struct NetmonHosts
{
	typedef std::vector< std::string > HostgroupList;
	typedef std::unordered_map< std::string, std::vector<std::string> > HostnameList;

	/// a list of all host groups
	HostgroupList hostGroups;
	/// a list of hostnames for each of the host groups
	Hostnamelist  hostnames;

	NetmonHosts( std::string filename )
		: hostnames( createHostnameTree ),
		  hostGroups( createHostGroupList(hostnames) )
	{}

private:
	/// derive the host group list from the host name list
	HostgroupList createHostgroupList( const HostnameList & hostnames ) const;

	/// read list of hosts from a netmon-hosts file
	HostnameList  createHostNameTree( const std::string & filename ) const;

	// print the hosts file
	friend std::ostream& operator<<( std::ostream&, const NetmonHosts& );
};

std::ostream& operator<<( std::ostream&, const NetmonHosts& );
