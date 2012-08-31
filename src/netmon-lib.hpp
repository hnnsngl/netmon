#ifndef  NETMON_LIB_INC
#define  NETMON_LIB_INC

#include <string>
#include <vector>
#include <utility>
#include <thread>
#include <future>
#include <unordered_map>
#include "netmon-types.hpp"
#include "socket.h"

/// refresh item "host", contact netmon-agent, build new item, lock mutexList, replace old item, unlock mutexList
void          refresh_HostListItem( const std::string& host, const int port );
/// create a new HostListItem,
HostListItem  build_HostListItem( const std::string& host, const int port );
/// refresh the complete hostList, using std::async to do this in different tasks for every item
void          refresh_HostList_blocking( const int port );

std::string   recv_full_message( const std::string& host, const int port );
HostListItem  sort_fullMessage( const std::string& host, const std::string& message );
void          print_HostList();

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
    std::cout << "thread id: " << std::this_thread::get_id() << "\n";
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
  HostListItem tmpitem(host);
  if( message.empty() ){
    return tmpitem;
  }

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

  auto getMemString = [&memory](const std::string& qual) -> std::string{
    size_t pos1, pos2;
    pos1 = memory.find(qual);
    pos2 = memory.substr(pos1, std::string::npos ).find("\n");
    pos1 = memory.substr(pos1, pos2).find_first_of("0123456789");
    return memory.substr(pos1, pos2-pos1);
  };
  tmpitem.memory = getMemString("MemTotal:");

  return tmpitem;
}

void print_HostList(){
  extern HostList hostList;
  for( auto &host: hostList ){
    std::cout << "#####" << host.first << "#####\n"
      << host.second.meminfo << "######\n" 
      << host.second.cpuinfo << "######\n" << host.second.memory << "\n";
  }
}


#endif   /* ----- #ifndef NETMON_LIB_INC  ----- */
