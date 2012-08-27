#include <iostream>

#include "netgroup.hpp"

int main( int argc, char** argv )
{
	// retrieve default host list for netmon
	std::vector<std::string> netmonHosts;
	for( const auto triple : getNetGroup( "workstation-all" ) )
		netmonHosts.push_back( std::get<0>(triple) );
	
}
