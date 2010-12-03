/* 
 * File:   GridNodeConsole.cpp
 * Author: HoelscJ
 * 
 * Created on 1. Oktober 2010, 13:50
 */

#include "GridNodeConsole.h"
#include <iostream>
#include "boost/bind.hpp"
namespace org {
  namespace esb {
    namespace grid {

      GridNodeConsole::GridNodeConsole() :
      send_endpoint_(boost::asio::ip::address::from_string("239.255.0.1"), 20200),
      send_socket_(send_service_, send_endpoint_.protocol()) {
        char line[256];
        std::string address=send_socket_.local_endpoint().address().to_string();

        std::cout << "mhive@";
        std::cout << address;
        std::cout <<" >>>";
        while (std::cin.getline(line, 256)) {
          if (strcmp(line, "quit") == 0)break;
          //std::cout << line << std::endl;
          send_socket_.async_send_to(
                  boost::asio::buffer(line), send_endpoint_,
                  boost::bind(&GridNodeConsole::handle_send, this,
                  boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
          
        std::cout << "mhive@";
        std::cout << address;
        std::cout <<" >>>";
        }
        std::cout << "bye"<<std::endl;
      }

      void GridNodeConsole::handle_send(const boost::system::error_code& error, std::size_t bytes_transferred) {

      }

      GridNodeConsole::~GridNodeConsole() {
      }
    }
  }
}

int main(int argc, char**argv) {
  org::esb::grid::GridNodeConsole c;
}
