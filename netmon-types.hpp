#include <map>
#include <vector>
#include <string>

struct processListItem {
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

	std::map<std::string, std::string> items;

	processListItem( std::string columns, std::string items );
};

struct hostListItem {
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

	std::vector< processListItem > processList;

	// hostListItem( std::string agentString );
	hostListItem( std::string hostname );
};

// std::vector< hostListItem > 
