#include "netmon-lib.hpp"
#include "socket.h"

#include <vector>
#include <utility>
#include <thread>
#include <future>
#include <limits>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <boost/regex.hpp>


void          refresh_HostListItem( const std::string& host, const int port ){
  extern HostList hostList;
  extern std::mutex mutexList;
  HostListItem tmp = build_HostListItem(host, port);
  mutexList.lock();
  hostList[ host ] = std::move(tmp);
  mutexList.unlock();
}

HostListItem  build_HostListItem( const std::string& host, const int port ){
  return sort_fullMessage( host, recv_full_message( host, port) );
}

std::string recv_full_message( const std::string& host, const int port ){
  Socket sock;
  sock.create();
  std::string s("");
  std::string message("");
  if( sock.connect( host , port )){
    int status = 1;
    while( (status = sock.recv(s)) != 0){
      message += s;
    }
  }
  sock.close();
  return message;
}

void refresh_HostList_blocking(const int port){
  extern HostList   hostList;
  extern std::mutex mutexList;

  auto refresh_host = [&hostList](const std::string& host, const int port) -> HostListItem{
    std::string message( recv_full_message( host, port ) );
    return sort_fullMessage( host, message );
  };
  std::unordered_map<std::string, std::future<HostListItem>> task_list;
  for(auto& host: hostList){
    task_list[host.first] = std::async( refresh_host, host.first, port);
  }
  for(auto& host: hostList){
    HostListItem tmp = task_list[host.first].get();
    mutexList.lock();
    host.second = std::move(tmp);
    mutexList.unlock();
  }
}

HostListItem sort_fullMessage(const std::string& host, const std::string& message){
  std::stringstream ss;
  HostListItem tmpitem(host);
  if( message.empty() ){
    return tmpitem;
  }
  tmpitem.alive = true;
  std::string flag_system("system");
  std::string flag_memory("memory");
  std::string flag_cpuinfo("cpuinfo");
  std::string flag_processes("processes");

  auto splitString = [&message](const std::string& flag) -> std::string {
    std::string begin = "--- " + flag + " ---";
    std::string end = "--- end " + flag + " ---";
    size_t pos1 = message.find( begin ) + begin.size() + 1;
    size_t pos2 = message.find( end );
    if( pos1 == std::string::npos or pos2 == std::string::npos ){
      std::cerr << "Something wrong with the netmon message.\n" << message << "\n";
      exit(1);
    }
    return message.substr(pos1, pos2-pos1);
  };
  std::string memory = splitString(flag_memory);
  std::string cpuinfo = splitString(flag_cpuinfo);
  std::string processes = splitString(flag_processes);
  std::string system = splitString(flag_system);
  
  tmpitem.meminfo = memory;
  tmpitem.cpuinfo = cpuinfo;

  // parse system
  // parse hostname
  tmpitem.hostname = system.substr(0, system.find_first_of("\n"));
  // parse agent-string
  boost::regex sysreg(R"(netmon-agent\s(\d+.\d+))");
  boost::smatch res;
  if( boost::regex_search(system, res, sysreg) )  tmpitem.agentversion = res[1];
  else                                            tmpitem.agentversion = "No Info";
  // parse uptime
  sysreg = (R"(up\s(\d+\s\w+))");
  if( boost::regex_search(system, res, sysreg) )  tmpitem.uptime = res[1];
  else                                            tmpitem.uptime = "No Info";
  // parse laod
  sysreg = R"(load average:\s(\d+.\d+),\s(\d+.\d+),\s(\d+.\d+))";
  if( boost::regex_search(system, res, sysreg) ){
    std::stringstream ss1, ss2, ss3;
    ss1 << res[1]; ss1 >> tmpitem.avgload1;
    ss2 << res[2]; ss2 >> tmpitem.avgload2;
    ss3 << res[3]; ss3 >> tmpitem.avgload3;
    tmpitem.load = res[1];
  }else{
    tmpitem.avgload1 = std::numeric_limits<double>::quiet_NaN();
    tmpitem.avgload2 = std::numeric_limits<double>::quiet_NaN();
    tmpitem.avgload3 = std::numeric_limits<double>::quiet_NaN();
    tmpitem.load = "NoInfo";
  }

  ss.str("");ss.clear();
  ss  << "load average 1: " << tmpitem.avgload1 << "\n"
      << "load average 2: " << tmpitem.avgload2 << "\n"
      << "load average 3: " << tmpitem.avgload3 << "\n";
  tmpitem.tooltip_load = ss.str();


  // parse memory
  auto getMemString = [&memory](const std::string& qual) -> std::string{
    size_t pos1, pos2;
    pos1 = memory.find(qual);
    pos2 = memory.substr(pos1, std::string::npos ).find("\n");
    pos1 = memory.substr(pos1, pos2).find_first_of("0123456789");
    return memory.substr(pos1, pos2-pos1);
  };
  tmpitem.memory = getMemString("MemTotal:");

  // parse cpuinfo
  // parse model name
  boost::regex  reg(R"(model name\s*:\s([a-zA-Z0-9/(/) ]*))");
  if( boost::regex_search(cpuinfo, res, reg) )  tmpitem.cpuname = res[1];
  else                                          tmpitem.cpuname = "No Info";
  // parse number of processors
  ss.str("");ss.clear();
  reg = R"(cpu cores\s*:\s(\d+))";
  if( boost::regex_search(cpuinfo, res, reg) )  {ss << res[1]; ss >> tmpitem.processors;}
  else                                          tmpitem.processors = std::numeric_limits<int>::quiet_NaN();

  reg = R"(cache size\s*:\s(\d+))";
  boost::regex_search(cpuinfo, res, reg);
  ss.str(""); ss.clear();
  ss << tmpitem.cpuname << "\n";
  ss << "Number cores: " << tmpitem.processors << "\n";
  ss << res[0] << "\n";
  tmpitem.tooltip_cpu = ss.str();
  

  // parsing process list - this is not be done by regex due to the very regular output from netmon-agent
  size_t pos1 = processes.find_first_of("\n");
  std::string header = processes.substr(0,pos1);
  processes.erase(0,pos1);
  bool open = false;
  pos1 = 0;
  for( size_t i=0; i < header.size(); ++i ){
    if( !open ){
      if( header[i] == '<' ){
        open = true;
        pos1 = i;
      }
    }else{
      if( header[i] == '>' ){
        open = false;
        tmpitem.HeaderProcessList.push_back(header.substr(pos1+1,i-pos1-1));
      }
    }
  }
  size_t numCol = tmpitem.HeaderProcessList.size();
  for( size_t i=0; i < numCol; ++i ){
    tmpitem.HeadToIndex[tmpitem.HeaderProcessList[i]] = i;
  }
  open = false;
  pos1 = 0;
  size_t elemCount = 0;
  ProcessListItem tmpPItem;
  tmpPItem.items.resize(numCol);
  for( size_t i=0; i < processes.size(); ++i ){
    if( !open ){
      if( processes[i] == '<' ){
        open = true;
        pos1 = i;
      }
    }else{
      if( processes[i] == '>' ){
        open = false;
        tmpPItem.items[elemCount] = processes.substr(pos1+1,i-pos1-1);
        ++elemCount;
      }
    }
    if( elemCount == numCol ){
      elemCount = 0;
      tmpitem.ProcessList.push_back(tmpPItem);
    }
  }

  return tmpitem;
}

void print_HostList( bool printProcesses ){
  extern HostList hostList;
  for( auto &host: hostList ){
    std::cout << "##### " << host.second.hostname << " #####\n";
    std::cout << "CPU: "  << host.second.cpuname << " - " << host.second.processors << " cores\n";
    std::cout << "RAM: " << host.second.memory << "\n";
    std::cout << "Uptime: " << host.second.uptime << "\n";
    std::cout << "Load: " << host.second.avgload1 << ", " << host.second.avgload2 << ", " << host.second.avgload3 << "\n";
    std::cout << "Agent version: " << host.second.agentversion << "\n";
    std::cout << "###### Processes ######\n";
    if( printProcesses ){
	    for( auto &head: host.second.HeaderProcessList ){
		    std::cout << ":" << head << ":\t";
	    }
	    std::cout << "\n";
	    for( auto &pList: host.second.ProcessList ){
		    for( auto &item: pList.items ){
			    std::cout << item << "\t";
		    }
		    std::cout << "\n";
	    }
    }
  }
}

