#include <iostream>
#include <fstream>

#include "netgroup.hpp"

int main( int argc, char** argv )
{
	// look for user host list file ~/.netmon-hosts
	std::ifstream if_netmonhosts(".netmon-hosts");
	if(if_netmonhosts) {

	}
	else {
		// retrieve default host list for netmon
		std::vector<std::string> netmonHosts;
		for( const auto triple : getNetGroup( "workstation-all" ) )
			netmonHosts.push_back( std::get<0>(triple) );
	}	

	// connect to hosts
	

	// read data


	// parse data


	// show data

}
