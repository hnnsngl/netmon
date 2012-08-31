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

void print_hostgroup(std::vector<std::string> & list)
{
	std::cout << std::endl;
	for( auto item : list )
		std::cout << " <<< " << item << " >>>" << std::endl;
	std::cout << std::endl;
}

HostnameList NetmonHosts::createHostNameTree( const std::string & filename ) const
{
	std::cerr << "parsing hosts file: " << filename << std::endl;

  std::ifstream in(filename.c_str(), std::ios_base::in);

  if (!in)
    {
      std::cerr << "Error: Could not open input file: "
                << filename << std::endl;
    }

  std::string data;							// We will read the contents here.
  in.unsetf(std::ios::skipws);	// No white space skipping!
  std::copy(
	    std::istream_iterator<char>(in),
	    std::istream_iterator<char>(),
	    std::back_inserter(data));

  std::cout << "--- host file ---" << std::endl
            << data
            << "--- end host file ---" << std:: endl;

  typedef std::string::iterator iterator;
  iterator begin = data.begin();
  iterator end   = data.end();
	
	// reading file with stream iterators
	// 
	// std::ifstream in( filename );
	// in.unsetf(std::ios::skipws);

	// // wrap istream into iterator
	// typedef boost::spirit::istream_iterator iterator;
	// iterator begin(in);
	// iterator end;

	using namespace boost::spirit;
	using namespace qi::ascii;
	namespace phx = boost::phoenix;
	// using qi::_1;

	using phx::ref;
	using phx::push_back;

	HostnameList map;
	auto insert = [&map](std::pair< std::string, std::vector<std::string> >& group)
		{
			map[group.first] = group.second;
		};

	qi::rule<iterator, std::string()> defgroup = lexeme['[' >> *omit[space] >> +( char_ - ']' ) >> ']'];
	qi::rule<iterator, std::string()> hostname = -omit[space] >> alpha >> *qi::alnum >> omit[space];
	qi::rule<iterator, std::string()> netgroup = lexeme[char_('@') >> alpha >> *( char_ - space)];
	// std::pair< std::string, std::vector<std::string> >()
	qi::rule<iterator, std::vector<std::string>()> hostgroup;
	hostgroup = +( hostname | netgroup );

	BOOST_SPIRIT_DEBUG_NODE(hostname);
	BOOST_SPIRIT_DEBUG_NODE(hostgroup);

	bool r = qi::phrase_parse( begin, end,
	                           hostgroup[&print_hostgroup],
	                           qi::space 	                           // ( char_('#') >>*( char_ - eol ) >> eol ) | space
	                           );

	// if( begin != end )
	// 	std::cerr << "Failed to parse hosts file: " << filename << std::endl;

	// if( !r )
	// 	std::cerr << "Failed to parse hosts file: " << filename << " (bad return value)" << std::endl;

  std::cout << "marker" << std::endl;

	return hostnames;
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
