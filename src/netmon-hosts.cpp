#include "netmon-hosts.hpp"
#include "netgroup.hpp"

#include <fstream>
#include <string>

#define BOOST_SPIRIT_DEBUG

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include<boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

HostgroupList NetmonHosts::createHostgroupList( const HostnameList & hostnames ) const
{
	HostgroupList groups;
	std::cout << "createHostgroupList" << std::endl;

	if(hostnames.size() > 0)
		std::cout << "looping " << hostnames.size()  << std::endl;
		for( const auto it : hostnames )
			groups.push_back(it.first);

	std::cout << "sorting groups" << std::endl;
	std::sort( groups.begin(), groups.end() );

	return groups;
}

void print_hostitems(std::vector<std::string> & list)
{
	std::cout << std::endl;
	for( auto item : list )
		std::cout << " <<< " << item << " >>>" << std::endl;
	std::cout << std::endl;
}

void print_defgroup(std::string & group)
{
	std::cout << " [[[ " << group << " ]]]" << std::endl;
}

void print_groupitem(std::pair< std::string, std::vector<std::string> > & list)
{
	std::cout << std::endl;
	std::cout << " [[[ " << list.first << " ]]] " << std::endl;
	for( auto item : list.second )
		std::cout << " <<< " << item << " >>> " << std::endl;
	std::cout << std::endl;
}

HostnameList NetmonHosts::createHostNameTree( const std::string & filename ) const
{
	using namespace boost::spirit;
	using namespace boost::spirit::qi::ascii;

	std::ifstream in( filename );
	in.unsetf(std::ios::skipws);

	// wrap istream into an input iterator
	typedef boost::spirit::istream_iterator iterator;
	iterator begin(in), end;

	qi::rule<iterator, std::string()> listitem, comment, hostitem, netgroup, hostgroup;
	listitem  %= comment | hostitem | netgroup | hostgroup ;
	comment %= ( *space >> char_('#') >> *( char_ - eol ) );
	hostitem  %= lexeme[ alpha >> *(alnum | punct) ];
	netgroup  %= lexeme[ (-char_('@')) >> alpha >> *(alnum | punct) ];
	hostgroup %= lexeme[ char_('[') >> +( char_ - ']' ) >> char_(']') ];

	qi::rule<iterator, std::vector<std::string>()> hostlist;
	hostlist  %= listitem % +space;

	std::vector<std::string> items;
	bool r = qi::phrase_parse( begin, end, hostlist, space, items );
	print_hostitems(items);

	if( (begin != end) || !r )
		std::cerr << "Failed to parse hosts file: " << filename << std::endl;

	// add items to our host map
	HostnameList list;
	std::string groupname = "Default Group";
	for( const auto item : items ){
		switch(item[0]){
		case '[':
		groupname = item;
		break;
		case '@':
		for(const auto triple : getNetGroupTriples( item.substr(1) ) )
			list[groupname].push_back(triple.hostname);
		break;
		case '#': continue;
		default:
		list[groupname].push_back(item);
		}
	}

	return HostnameList();
}

std::ostream& operator<< ( std::ostream & os, const NetmonHosts & netmonHosts )
{
	for( const auto group : netmonHosts.hostnames ){
		os << "[" << group.first << "]" << std::endl;

		for( const auto host : group.second )
			os << host << std::endl;

		os << std::endl;
	}

	return os;
}
