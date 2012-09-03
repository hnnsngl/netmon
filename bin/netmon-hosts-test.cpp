#include <string>
#include <netmon-hosts.hpp>
#include <iostream>

int main()
{
	NetmonHosts netmonHosts( "/home/hnagel/.netmon-hosts" );
	std::cout << "Printing parsed netmon-hosts file:" << std::endl;
}
