#pragma once

#include <string>
#include <unordered_map>

/**
 *  Host selection manager used for filtering hosts in the process
 *  lists. Allows for multiple selections and group
 *  selections. Resolves names of selected items to their canonical
 *  names using NetmonHosts (useful for multiple selections where the
 *  items are aliases for the same host)
 */
class HostSelections
{
      private:
	/** Reference to Host hierarchy structure */
	NetmonHosts &netmonHosts;
	/** selection count for each host item */
	std::unordered_map<std::string, int> items;
	/** selection status for host groups */
	std::unordered_map<std::string, bool> groups;

      public:
	HostSelections(NetmonHosts &hosts) : netmonHosts(hosts) {}

	/** Select a single host item. */
	void selectHost(std::string hostname, bool enabled)
	{
		// check/sanitize hostname
		if (enabled)
			items[hostname]++;
		else
			items[hostname]--;
	}

	/** Select or deslect an entire group of hosts. */
	void selectGroup(std::string hostgroup, bool enabled)
	{
		for (const auto &hostname : netmonHosts.hostnames[hostgroup])
			selectHost(hostname, enabled);

		groups[hostgroup] = enabled;
	}

	/** Return whether a specific hostname is selected. */
	bool operator[](std::string hostname) { return (items[hostname] > 0); }

	/** Return whether the selection list is empty. Returns true, if
	 *  nothing is selected including groups */
	bool empty() const
	{
		for (const auto host : items)
			if (host.second != 0)
				return false;

		for (const auto group : groups)
			if (group.second)
				return false;

		return true;
	}

	/** print all selections */
	std::ostream &print(std::ostream &os) const
	{
		for (const auto &item : items)
			os << item.first << "\t" << item.second << std::endl;
		return os;
	}
};
