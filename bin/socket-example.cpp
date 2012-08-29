#include "../src/socket.h"
#include <string>
#include <iostream>

std::string recv_full_message( const std::string& host, const std::string& port ){
  Socket sock;
  sock.create();
  sock.connect( host , stoi(port) );
  int status = 1;
  std::string s("");
  std::string message("");
  while( (status = sock.recv(s)) != 0){
    message += s;
  }
  return message;
  sock.close();
}

int main ( int argc, char *argv[] )
{
  if( argc != 3 ){
    std::cout << "Usage: " << *argv << " adress port\n";
    exit(1);
  }
  std::cout << recv_full_message( argv[1], argv[2] );
  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
