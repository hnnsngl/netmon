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

	std::unordered_map<std::string, std::string> items;

	ProcessListItem()
	  : items({ {"UID", "NoInfo"}, {"PID", "NoInfo"} })
    {}
};

struct HostListItem {
	std::string hostname;
	std::string cpuname;					// "Core2", "Athlon", ...
	std::string memory;						// "4020MB"
	std::string uptime;						// "22 days, 11:31"

	int processors;
	double avgload1;
	double avgload2; 
	double avgload3; 

	std::string cpuinfo;					// complete /dev/cpuinfo for tooltip or something
	std::string meminfo;					// complete /dev/meminfo for tooltip or something

  std::vector<ProcessListItem>  ProcessList;

	HostListItem()
	  : hostname("None"), cpuname("NoInfo"), memory("NoInfo"), uptime("NoInfo"),
	    processors(1), avgload1(0), avgload2(0), avgload3(0), 
	    cpuinfo("NoInfo"), meminfo("NoInfo")
  {}
	HostListItem( const std::string& hostname_ )
	  : hostname(hostname_), cpuname("NoInfo"), memory("NoInfo"), uptime("NoInfo"),
	    processors(1), avgload1(0), avgload2(0), avgload3(0), 
	    cpuinfo("NoInfo"), meminfo("NoInfo")
  {}
};

typedef std::unordered_map< std::string, HostListItem > HostList;
