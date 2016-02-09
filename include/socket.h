#ifndef SOCKET_INC
#define SOCKET_INC

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <exception>

const int MAXCONNECTIONS = 5;
const int MAXRECV = 1024;

class Socket
{
      private:
	/// socket number ( socket-descriptor )
	int m_sock;
	/// struct sockaddr_in
	sockaddr_in m_addr;

      public:
	/// constructor
	Socket();
	/// virtuel destructor
	virtual ~Socket();

	/// create socket - TCP
	bool create();
	bool bind(const int port);
	bool listen() const;
	bool accept(Socket &) const;
	bool connect(const std::string host, const int port);
	bool send(const std::string) const;
	int recv(std::string &) const;
	bool close() const;
	void cleanup() const;
	bool is_valid() const { return m_sock != -1; }
};

class SockExcept
{
      private:
	std::string exception;

      public:
	SockExcept(const std::string &s) : exception(s){};
	virtual ~SockExcept(){};
	virtual const char *what() const throw() { return exception.c_str(); }
};

#endif /* ----- #ifndef SOCKET_INC  ----- */
