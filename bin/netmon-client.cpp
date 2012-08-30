#include <iostream>
#include <fstream>
#include <string>
#include <future>
#include <functional>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <boost/program_options.hpp>

#include "../src/socket.h"
#include "../src/netgroup.hpp"
#include "../src/netmon-types.hpp"

namespace po = boost::program_options;

std::string   recv_full_message( const std::string& host, const int port );
void          refresh_HostList_blocking(const int port);
HostListItem  sort_fullMessage(const std::string& host, const std::string& message);
void          print_HostList();

HostList                                           hostList;
std::unordered_map< std::string, std::timed_mutex> mutexList;

int main( int argc, char** argv )
{
	std::vector<std::string>  netmonHosts;
	int                       numHosts;
  std::string               netGroup;
  int                       port;

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
      for( const auto triple : getNetGroup( netGroup ) )
        netmonHosts.push_back( std::get<0>(triple) );
    }
  }
  numHosts = netmonHosts.size();
  for( auto &host: netmonHosts ){
    hostList[host] = HostListItem(host);
  }
  print_HostList();
  refresh_HostList_blocking(port);
  print_HostList();


	// show data

}

std::string recv_full_message( const std::string& host, const int port ){
  Socket sock;
  sock.create();
  sock.connect( host , port );
  int status = 1;
  std::string s("");
  std::string message("");
  while( (status = sock.recv(s)) != 0){
    message += s;
  }
  return message;
  sock.close();
}

void          refresh_HostList_blocking(const int port){
  for(auto& host: hostList){
    std::string message( recv_full_message( host.first, port ) );
    HostListItem tmp = sort_fullMessage( host.first, message );
    hostList[host.first] = tmp;
  }
}

HostListItem sort_fullMessage(const std::string& host, const std::string& message){
  HostListItem tmpitem(host);

  size_t pos1, pos2;
  pos1 = message.find("--- system ---\n");
  pos2 = message.find("--- end system\n");
  if( pos1 == std::string::npos or pos2 == std::string::npos ){
    std::cerr << "Something wrong with the netmon message.\n" << message << "\n";
    exit(1);
  }
  std::string system = message.substr(pos1, pos2-1);
  pos1 = message.find("--- cpuinfo ---\n");
  pos2 = message.find("--- end cpuinfo ---\n");
  if( pos1 == std::string::npos or pos2 == std::string::npos ){
    std::cerr << "Something wrong with the netmon message.\n" << message << "\n";
    exit(1);
  }
  std::string cpuinfo = message.substr(pos1, pos2-1);
  pos1 = message.find("--- cpuinfo ---\n");
  pos2 = message.find("--- end cpuinfo ---\n");
  if( pos1 == std::string::npos or pos2 == std::string::npos ){
    std::cerr << "Something wrong with the netmon message.\n" << message << "\n";
    exit(1);
  }
  std::string memory = message.substr(pos1, pos2-1);
  pos1 = message.find("--- processes ---\n");
  pos2 = message.find("--- end processes ---\n");
  if( pos1 == std::string::npos or pos2 == std::string::npos ){
    std::cerr << "Something wrong with the netmon message.\n" << message << "\n";
    exit(1);
  }
  std::string processes = message.substr(pos1 ,pos2-1);

  tmpitem.meminfo = memory;
  tmpitem.cpuinfo = cpuinfo;

  auto getMemString = [&memory](const std::string& qual) -> std::string{
    size_t pos1, pos2;
    pos1 = memory.find(qual);
    pos2 = memory.substr(pos1).find("\n");
    pos1 = memory.substr(pos1, pos2).find_first_of("0123456789");
    pos2 = memory.substr(pos1, pos2).find("kB");
    return memory.substr(pos1, pos2+1);
  };
  tmpitem.memory = getMemString("MemTotal:");

  return tmpitem;
}

void print_HostList(){
  for( auto &host: hostList ){
    std::cout << host.first << ": " << host.second.cpuinfo << host.second.memory << "\n";
  }
}
