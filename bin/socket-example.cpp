#include "../src/socket.h"
#include <string>
#include <iostream>
#include <future>
#include <functional>
#include <vector>

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
  if( argc < 2 ){
    std::cout << "Usage: " << *argv << " port adress1 adress2 ...\n";
    exit(1);
  }
  int number_adresses = argc - 1;
  std::vector< std::future<std::string> > tasks;
  for(int i = 0; i < number_adresses; ++i){
    tasks.push_back( async( std::bind( recv_full_message, argv[2], argv[1]) ) );
  }
  for(int i = 0; i < number_adresses; ++i){
    std::cout << tasks[i].get();
    std::cout << "##############################################\n";

  }
  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */