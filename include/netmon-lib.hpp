#pragma once

#include "netmon-types.hpp"

#include <string>

/// refresh item "host", contact netmon-agent, build new item, lock
/// mutexList, replace old item, unlock mutexList

/** one can run this serial: refresh_HostListItem(host, port) in a
 *  thread std::thread t1(refresh_HostListItem, host, port), t1.join()
 *  will synchronize , t1.detach will detach the thread in a task auto
 *  task = std::async(refresh_HostListItem, host, port)*. task.get()
 *  will synchronize */
void refreshHostListItem(const std::string &host, const int port);

/// refresh the complete hostList, using std::async to do this in
/// different tasks for every key in the hostList
void refreshHostListBlocking(const int port);

/// create a new HostListItem,
HostListItem buildHostListItem(const std::string &host, const int port);

/// connect to host and receive message
std::string receiveFullMessage(const std::string &host, const int port);

///
HostListItem sortFullMessage(const std::string &host, const std::string &message);

///
void printHostList(bool printProcesses = false);
