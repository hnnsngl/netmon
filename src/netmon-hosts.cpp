#include "netmon-hosts.hpp"
#include "netgroup.hpp"

#include <algorithm>
#include <fstream>
#include <locale>
#include <regex>
#include <string>

HostgroupList NetmonHosts::createHostgroupList(const HostnameList &hostnames) const
{
	HostgroupList groups;
	if (hostnames.size() > 0) {
		for (const auto &it : hostnames) {
			groups.push_back(it.first);
		}
	}
	std::sort(groups.begin(), groups.end());
	return groups;
}

std::vector<std::string> parseHostlistFile(std::string filename)
{
	std::locale loc;
	std::vector<std::string> list;

	std::regex pattern(R"(^[[:s:]]*([\@]?[[:alpha:]][\w\.\-]+|\[.+\])[[:s:]]*)", std::regex_constants::ECMAScript);
	std::ifstream in(filename);
	// if file does not exist, create file contents
	// TODO: do error handling for files
	if (!in.good()) {
		in.close();
		std::cerr << "Creating default netmon configuration: " << filename << ". Edit " << filename
		          << " to customize." << std::endl;
		std::ofstream newfile(filename);
		newfile << "# [ group (optional) ]\n# @netgroup\n# hostname\n" << std::endl
		        << "[ All Workstations ]\n@workstation-all" << std::endl
		        << "[ CQT Workstations ]\n@workstation-CQT" << std::endl << "[ Other ]\nbobrowski\nbenjamin"
		        << std::endl;
		in.open(filename);
		// ignore any errors following this ...
	}

	// read line by line, push matches to list to be processed later
	std::string input;
	while (std::getline(in, input)) {
		std::smatch match;
		while (std::regex_search(input, match, pattern)) {
			list.push_back(match[1]);
			input.erase(0, match.length());
		}
	}
	return list;
}

HostnameList NetmonHosts::createHostNameTree(const std::string &filename) const
{
	// read tokens from hostlist file
	std::vector<std::string> items = parseHostlistFile(filename);
	// add items to our host map
	HostnameList list;
	std::string groupname = "Default Group";
	// list[groupname] = std::vector<std::string>();
	for (const auto &item : items) {
		switch (item[0]) {
		case '[':
			groupname = item.substr(1, item.size() - 2);
			;
			// TODO we could cut away leading and trailing whitespace now

			// list[groupname] = std::vector<std::string>();
			break;
		case '@':
			for (const auto &triple : NetGroup(item.substr(1))) {
				if (triple.hostname.size() > 0)
					list[groupname].push_back(triple.hostname);
			}
			break;
		case '#':
			continue;
		default:
			if (item.size() > 0) {
				list[groupname].push_back(item);
			}
		}
	}

	// add additional default group : all hosts
	// groupname = "All Hosts";
	// std::set<std::string> allhosts;
	// for( const auto& group : list )
	// 	for( const auto& host : group.second )
	// 		allhosts.insert(host);
	// for( const auto& host : allhosts )
	//   list[groupname].push_back(host);

	// try to sort the individual host lists
	for (auto &group : list) {
		std::sort(group.second.begin(), group.second.end());
	}
	return std::move(list);
}

std::ostream &operator<<(std::ostream &os, const NetmonHosts &netmonHosts)
{
	std::cout << "# Netmon Hosts " << netmonHosts.hostnames.size() << std::endl;
	for (const auto &group : netmonHosts.hostnames) {
		os << "[" << group.first << "]" << std::endl;

		for (const auto &host : group.second)
			os << host << std::endl;
		// os << "##END group" << std::endl;
	}

	// os << "##HOSTGROUPS" << netmonHosts.hostGroups.size() << std::endl;
	// for( const auto group : netmonHosts.hostGroups )
	// 	os << group << std::endl;
	// os << "END" << std::endl;

	return os;
}
