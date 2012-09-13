#include "netmon-hosts.hpp"
#include "netgroup.hpp"

#include <boost/regex.hpp>

#include <algorithm>
#include <fstream>
#include <string>
#include <locale>

HostgroupList NetmonHosts::createHostgroupList( const HostnameList & hostnames ) const
{
	HostgroupList groups;
	if(hostnames.size() > 0)
		for( const auto & it : hostnames )
			groups.push_back(it.first);

	std::sort( groups.begin(), groups.end() );
	return groups;
}

std::vector<std::string> parseHostlistFile( std::string filename )
{
	std::locale loc;
	std::vector<std::string> list;

	// boost::regex pattern (R"([:alpha:][^ \t\n]*|@[:alpha:][^ \t\n#]*|\[[]^]\]|#.*$|)");
	boost::regex pattern
		(R"(^[[:s:]]*([\@]?[[:alpha:]][\w\.\-]+|\[.+\])[[:s:]]*)", boost::regex::perl);
	std::ifstream in(filename);
	std::string input;
	while( std::getline( in, input ) ){
		boost::smatch match;
		while( boost::regex_search( input, match, pattern ) ) {
			list.push_back(match[1]);
			input.erase(0, match.length());
		}
	}
	return list;
}

HostnameList NetmonHosts::createHostNameTree( const std::string & filename ) const
{
	// read tokens from hostlist file
	std::vector<std::string> items = parseHostlistFile( filename );
	// add items to our host map
	HostnameList list;
	std::string groupname = "Default Group";
	list[groupname] = std::vector<std::string>();
	for( const auto & item : items ){
		switch(item[0]){
		case '[':
			groupname = item.substr(1,item.size()-2);;
			// TODO we could cut away leading and trailing whitespace now
			list[groupname] = std::vector<std::string>();
			break;
		case '@':
			for(const auto & triple : getNetGroupTriples( item.substr(1) ) ){
				if(triple.hostname.size() > 0)
					list[groupname].push_back(triple.hostname);
			}
			break;
		case '#': continue;
		default:
			if(item.size() > 0)			
				list[groupname].push_back(item);
		}
	}

	// // add additional default group : all hosts
	// groupname = "All Hosts";
	// for( const auto& group : list )
	// 	for( const auto& host : group.second )
	// 		list[groupname].push_back(host);

	// try to sort the individual host lists
	for( auto& group : list )
		std::sort( group.second.begin(), group.second.end() );

	return std::move(list);
}

std::ostream& operator<< ( std::ostream & os, const NetmonHosts & netmonHosts )
{
	std::cout << "# Netmon Hosts " << netmonHosts.hostnames.size() << std::endl;
	for( const auto & group : netmonHosts.hostnames ){
		os << "[" << group.first << "]" << std::endl;

		for( const auto & host : group.second )
			os << host << std::endl;
		// os << "##END group" << std::endl;
	}

	// os << "##HOSTGROUPS" << netmonHosts.hostGroups.size() << std::endl;
	// for( const auto group : netmonHosts.hostGroups )
	// 	os << group << std::endl;
	// os << "END" << std::endl;

	return os;
}
