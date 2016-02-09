#pragma once

#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>

typedef std::vector<std::string> HostgroupList;
typedef std::unordered_map<std::string, std::vector<std::string>> HostnameList;
typedef std::unordered_map<std::string, int> HostSelectedList;

struct NetmonHosts
{
	/// a list of hostnames for each of the host groups
	HostnameList hostnames;

	/// a list of all host groups
	HostgroupList hostGroups;

	NetmonHosts(std::string filename)
	    : hostnames(createHostNameTree(filename)), hostGroups(createHostgroupList(hostnames))
	{
	}

      private:
	/** read list of hosts from a netmon-hosts file with the following simple format
	 *  - comments start with a hash # symbol and continue the rest of the line
	 *  - a hostgroup is declared via brackets "[My favourite hosts]" followed by hosts
	 *  - a default hostgroup is declared implicitly
	 *  - declared hosts are added to the last hostgroup declared
	 *  - hosts may be declared in multiple hostgroups
	 *  - a host may be hostname ("eucken")  or a netgroup ("@workstation-all")
	 *  - host and netgroup declarations must be separated by whitespace
	 */
	HostnameList createHostNameTree(const std::string &filename) const;

	/** derive the host group list from the host name list */
	HostgroupList createHostgroupList(const HostnameList &hostnames) const;

	// print the hosts file
	friend std::ostream &operator<<(std::ostream &, const NetmonHosts &);
};

/// print the unsorted list of hostgroups in the netmon-hosts file format
std::ostream &operator<<(std::ostream &, const NetmonHosts &);
