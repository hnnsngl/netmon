#include <iostream>
#include <fstream>
#include <string>
#include <future>
#include <functional>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <boost/program_options.hpp>

#include "socket.h"
#include "netgroup.hpp"
#include "netmon-types.hpp"
#include "netmon-lib.hpp"

namespace po = boost::program_options;

HostList   hostList;
std::mutex mutexList;

int main( int argc, char** argv )
{
	std::vector<std::string>  netmonHosts;
  std::string               netGroup;
  int                       port;

  try{
  po::options_description param("Parameter");
  param.add_options()
    ("help,h", "produce this help message")
    ("NetMon-Hosts,n", po::value< std::vector<std::string> >(&netmonHosts)->multitoken(),
     "list of hist to check, if not given ~/.netmon-hosts is taken, if this file not present netgroup: ${NetGroup} is taken")
    ("NetGroup,N", po::value< std::string >(&netGroup)->default_value("workstation-all"), "which netgroup is taken as fallback for hosts")
    ("port,p", po::value< int >(& port)->default_value(9221), "port to connect to")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, param), vm);
  po::notify(vm);

  if(vm.count("help")){
    std::cout << param << "\n";
    exit(1);
  }
  if(not vm.count("NetMon-Hosts")){
    // look for user host list file ~/.netmon-hosts
    std::ifstream if_netmonhosts(".netmon-hosts");
    if(if_netmonhosts) {
      std::cerr << "Not implemented\n";
      exit(1);
    }
    else {
      // retrieve default host list for netmon
      //for( const auto triple : getNetGroup( netGroup ) )
      //  netmonHosts.push_back( std::get<0>(triple) );
      exit(1);
    }
  }

  for( auto &host: netmonHosts ){
    hostList[host] = HostListItem(host);
  }
  refresh_HostList_blocking(port);
  print_HostList();
  }
  catch( std::exception& e ){
    std::cerr << e.what();
  }
  catch( SockExcept& e ){
    std::cerr << e.what();
  }

	// show data
}
