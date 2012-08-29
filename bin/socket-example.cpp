#include "../src/socket.h"
#include <string>
#include <iostream>

int main ( int argc, char *argv[] )
{
  if( argc != 2 ){
    std::cout << "Usage: " << *argv << " server adress\n";
    exit(1);
  }
  Socket sock;
  sock.create();
  std::string argv_1 = argv[1];
  sock.connect( argv_1, 9221 );
  std::cout << sock.recv_until_close() << std::endl;
  sock.close();
  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
