#ifndef  NETMON_TYPES_INC
#define  NETMON_TYPES_INC

#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>

struct ProcessListItem {
	// UID    username
	// PID    process id
	// PPID   parent process id
	// LWP    thread id
	// C      CPU utilization
	// NLWP   number of threads
	// SZ     
	// RSS
	// PSR
	// STIME  processor time
	// TTY    terminal
	// TIME   
	// CMD

	std::vector<std::string> items;
};

struct HostListItem {
  bool alive;

	std::string hostname;
	std::string cpuname;					// "Core2", "Athlon", ...
	std::string memory;						// "4020MB"
	std::string load;
	std::string uptime;						// "22 days, 11:31"
  std::string agentversion;

	int processors;
	double avgload1;
	double avgload2; 
	double avgload3; 

	std::string cpuinfo;					// complete /dev/cpuinfo for tooltip or something
	std::string meminfo;					// complete /dev/meminfo for tooltip or something

  std::string tooltip_cpu;
  std::string tooltip_load;

  std::vector<std::string>                HeaderProcessList;
  std::unordered_map<std::string, size_t> HeadToIndex;
  std::vector<ProcessListItem>            ProcessList;

	HostListItem()
		: alive(false), hostname("None"), cpuname("NoInfo"), memory("NoInfo"), load("NoInfo"),
		  uptime("NoInfo"),  processors(1), avgload1(0), avgload2(0), avgload3(0), 
	    cpuinfo("NoInfo"), meminfo("NoInfo")
  {}
	explicit HostListItem( const std::string& hostname_ )
	  : alive(false), hostname(hostname_), cpuname("NoInfo"), memory("NoInfo"), load("NoInfo"), 
	    uptime("NoInfo"), processors(1), avgload1(0), avgload2(0), avgload3(0), 
	    cpuinfo("NoInfo"), meminfo("NoInfo"), tooltip_cpu(""), tooltip_load("")
  {}
};

typedef std::unordered_map< std::string, HostListItem > HostList;

#endif   /* ----- #ifndef NETMON_TYPES_INC  ----- */
