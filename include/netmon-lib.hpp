#ifndef  NETMON_LIB_INC
#define  NETMON_LIB_INC

#include "netmon-types.hpp"

#include <string>

/// refresh item "host", contact netmon-agent, build new item, lock mutexList, replace old item, unlock mutexList
/** one can run this serial: refresh_HostListItem(host, port)
 *  in a thread std::thread t1(refresh_HostListItem, host, port), t1.join() will synchronize , t1.detach will detach the thread
 *  in a task auto task = std::async(refresh_HostListItem, host, port)*. task.get() will synchronize */
void          refresh_HostListItem( const std::string& host, const int port );
/// refresh the complete hostList, using std::async to do this in different tasks for every key in the hostList
void          refresh_HostList_blocking( const int port );

/// create a new HostListItem,
HostListItem  build_HostListItem( const std::string& host, const int port );
std::string   recv_full_message( const std::string& host, const int port );
HostListItem  sort_fullMessage( const std::string& host, const std::string& message );
void          print_HostList( bool printProcesses = false );


#endif   /* ----- #ifndef NETMON_LIB_INC  ----- */
