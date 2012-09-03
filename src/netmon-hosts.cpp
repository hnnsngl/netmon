#include "netmon-hosts.hpp"

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
	std::cerr << "parsing hosts file: " << filename << std::endl;

	std::ifstream in( filename );
	in.unsetf(std::ios::skipws);

	// wrap istream into iterator
	typedef boost::spirit::istream_iterator iterator;
	iterator begin(in), end;

	using namespace boost::spirit;
	using namespace boost::spirit::qi::ascii;

	// qi::rule<iterator, space_type, std::string()> defgroup;
	// defgroup %= lexeme['[' >> *(alpha - ']') >> ']'];

	qi::rule<iterator, std::vector<std::string>()> hostlist;
	hostlist %= lexeme[ (-char_('@')) >> alpha >> *(alnum | punct) ] % +space;

	// qi::rule<iterator, std::vector<std::string>()> hostgroup;
	// hostgroup %= defgroup >> lexeme[ (-char_('@')) >> alpha >> *(alnum | punct) ] % +space;

	BOOST_SPIRIT_DEBUG_NODE(hostlist);
	BOOST_SPIRIT_DEBUG_NODE(defgroup);

	std::vector<std::string> map;
	bool r = qi::phrase_parse( begin, end, hostlist, space, map);
	print_hostitems(map);

	if( (begin != end) || !r )
		std::cerr << "Failed to parse hosts file: " << filename << std::endl;

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
