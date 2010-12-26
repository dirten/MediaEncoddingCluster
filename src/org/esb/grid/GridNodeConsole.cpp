/* 
 * File:   GridNodeConsole.cpp
 * Author: HoelscJ
 * 
 * Created on 1. Oktober 2010, 13:50
 */

#include "GridNodeConsole.h"
#include <iostream>
#include "org/esb/net/TcpSocket.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace grid {

      GridNodeConsole::GridNodeConsole() {
        org::esb::net::TcpSocket socket("localhost", 2020);
        socket.connect();
        std::string data="test";
        socket.getOutputStream()->write(data);
        socket.close();
    }


      GridNodeConsole::~GridNodeConsole() {
      }
    }
  }
}

int main(int argc, char**argv) {
  Log::open();
  org::esb::grid::GridNodeConsole c;
}
