#include "netmon-hosts.hpp"
#include "netgroup.hpp"

#include <fstream>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include<boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

HostgroupList NetmonHosts::createHostgroupList( const HostnameList & hostnames ) const
{
	HostgroupList groups;
	if(hostnames.size() > 0)
		for( const auto it : hostnames )
			groups.push_back(it.first);

	std::sort( groups.begin(), groups.end() );
	return groups;
}

std::vector<std::string> parseHostlistFile( std::string filename ){
	using namespace boost::spirit;
	using namespace boost::spirit::qi::ascii;

	std::ifstream in( filename );
	in.unsetf(std::ios::skipws);

	// wrap istream into an input iterator
	typedef boost::spirit::istream_iterator iterator;
	iterator begin(in), end;

	qi::rule<iterator, std::string()> listitem, comment, hostitem, netgroup, hostgroup;
	listitem  %= comment | hostitem | netgroup | hostgroup ;
	comment   %= ( *space >> char_('#') >> *( char_ - eol ) );
	hostitem  %= lexeme[ alpha >> *(alnum | punct) ];
	netgroup  %= lexeme[ (-char_('@')) >> alpha >> *(alnum | punct) ];
	hostgroup %= lexeme[ char_('[') >> +( char_ - ']' ) >> char_(']') ];

	std::vector<std::string> items;
	qi::rule<iterator, std::vector<std::string>()> hostlist;
	hostlist  %= listitem % +space;

	bool r = qi::phrase_parse( begin, end, hostlist, space, items );

	if( (begin != end) || !r )
		std::cerr << "Failed to parse hosts file: " << filename << std::endl;

	return std::move(items);
}

HostnameList NetmonHosts::createHostNameTree( const std::string & filename ) const
{
	std::vector<std::string> items = parseHostlistFile( filename );

	// add items to our host map
	HostnameList list;
	std::string groupname = "Default Group";
	list[groupname] = std::vector<std::string>();
	for( const auto item : items ){
		switch(item[0]){
		case '[':
			groupname = item;
			list[groupname] = std::vector<std::string>();
			break;
		case '@':
			for(const auto triple : getNetGroupTriples( item.substr(1) ) ){
				list[groupname].push_back(triple.hostname);
			}
			break;
		case '#': continue;
		default:
			list[groupname].push_back(item);
		}
	}

	for( auto group : list )
		std::sort( group.second.begin(), group.second.end() );

	return std::move(list);
}

std::ostream& operator<< ( std::ostream & os, const NetmonHosts & netmonHosts )
{
	std::cout << "# Netmon Hosts " << netmonHosts.hostnames.size() << std::endl;
	for( const auto group : netmonHosts.hostnames ){
		os << "[" << group.first << "]" << std::endl;

		for( const auto host : group.second )
			os << host << std::endl;

		os << std::endl;
	}

	return os;
}
