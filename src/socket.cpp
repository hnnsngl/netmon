#include <cstdlib>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "socket.h"

/// constructor
Socket::Socket()
  : m_sock(0)
{}

/// destructor
Socket::~Socket() {
  if (is_valid() ){
    /// leading :: - global namespace instead of Socket::close() (C-API)
    ::close( m_sock );
  }
}

bool Socket::create() {
  /// leading :: - global namespace
  m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
  if( m_sock < 0 ){
    throw SockExcept("Error on creation of socker.");
  }
  int y=1;
  setsockopt( m_sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
  return true;
}

bool Socket::bind( const int port ){
  if( ! is_valid() ){
    return false;
  }
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );
  int bind_return = ::bind( m_sock, ( struct sockaddr* ) &m_addr, sizeof( m_addr ));
  if ( bind_return == -1 ){
    return false;
  }
  return true;
}

/// tell socket that connections from clients are accepted
bool Socket::listen() const{
  if( ! is_valid() ){
    return false;
  }
  int listenn_return = ::listen( m_sock, MAXCONNECTIONS );
  if( listenn_return == -1 ){
    return false;
  }else{
    return true;
  }
}

/// work at connections from clients, blocking call
bool Socket::accept( Socket& new_socket ) const {
  int addr_length = sizeof( m_addr );
  new_socket.m_sock = ::accept( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );
  if( new_socket.m_sock <= 0 ){
    return false;
  }else{
    return true;
  }
}

/// create connections to server
bool Socket::connect( const std::string host, const int port ){
  if ( ! is_valid() ){
    return false;
  }
  struct hostent *host_info;
  unsigned long addr;
  memset( &m_addr, 0, sizeof(m_addr));
  if ((addr = inet_addr( host.c_str() )) != INADDR_NONE) {
    // argument ist numerical ip adress
    memcpy( (char *) &m_addr.sin_addr, &addr, sizeof(addr) );
  }else{
    // convert adress in ip-address
    host_info = gethostbyname( host.c_str() );
    std::cerr << host_info->h_name << "\n";
    if( host_info == NULL ){
      return false;
    }
    memcpy( (char *) &m_addr.sin_addr, host_info->h_addr, host_info->h_length);
  }
  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons( port );

  int status = ::connect( m_sock, ( sockaddr * ) &m_addr, sizeof( m_addr ));
  if ( status != 0 ){
    return false;
  }else{
    return true;
  }
}

/// send data via TCP
bool Socket::send( const std::string s ) const {
  int status = ::send( m_sock, s.c_str(), s.size(), 0 );
  if( status == -1 ) {
    return false;
  }else {
    return true;
  }
}

/// recive data via TCP
/// returns 0 if connection is closed
int Socket::recv( std::string& s ) const {
  char buf [ MAXRECV + 1 ];
  s = "";
  memset( buf, 0, MAXRECV +1);
  int status = ::recv( m_sock, buf, MAXRECV, 0);
  if( status > 0 || status != -1 ){
    s = buf;
    return status;
  }else{
    throw("Error in Socket::recv");
  }
}

void Socket::cleanup() const {}

/// close socket
bool Socket::close() const {
  ::close(m_sock);
  cleanup();
  return true;
}
