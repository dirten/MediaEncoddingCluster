/* 
 * File:   TestNodeDiscovery.cpp
 * Author: jhoelscher
 *
 * Created on 7. Dezember 2009, 14:39
 */

#include <stdlib.h>

#include "org/esb/hive/NodeResolver.h"
#include "org/esb/lang/Thread.h"
/*
 * 
 */
using namespace org::esb::hive;
using namespace org::esb::lang;

int main(int argc, char** argv) {
  NodeResolver res(boost::asio::ip::address::from_string("0.0.0.0"),boost::asio::ip::address::from_string("239.255.0.1"), 6000);
//  NodeReceiver receiver(boost::asio::ip::address::from_string("0.0.0.0"),boost::asio::ip::address::from_string("239.255.0.1"));
//  NodeSender sender(boost::asio::ip::address::from_string("239.255.0.1"));

  res.start();

  Thread::sleep2(15000);
  return (EXIT_SUCCESS);
}

