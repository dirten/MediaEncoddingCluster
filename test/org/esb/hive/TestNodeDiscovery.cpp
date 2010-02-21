/* 
 * File:   TestNodeDiscovery.cpp
 * Author: jhoelscher
 *
 * Created on 7. Dezember 2009, 14:39
 */

#include <stdlib.h>

#include "org/esb/hive/NodeResolver.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
/*
 * 
 */
using namespace org::esb::hive;
using namespace org::esb::lang;
class NodeAgent: public NodeListener{
  classlogger("NodeListener")
public:

  void onNodeUp(Node & node){
    LOGDEBUG("NodeUp:"<<node.toString());
    LOGDEBUG("NodeData "<<node.getData("type"));
  }
  
  void onNodeDown(Node & node){
    LOGDEBUG("NodeDown:"<<node.toString());
  }
};

int main(int argc, char** argv) {
  Log::open("");

  Node node;
  if(argc>1)
    node.setData("test1","test2");
  else{
    node.setData("test1","test1");
  }

  NodeResolver res(boost::asio::ip::address::from_string("0.0.0.0"),boost::asio::ip::address::from_string("239.255.0.1"), 6000, node);
//  NodeReceiver receiver(boost::asio::ip::address::from_string("0.0.0.0"),boost::asio::ip::address::from_string("239.255.0.1"));
//  NodeSender sender(boost::asio::ip::address::from_string("239.255.0.1"));
//  res.setNode(node);
  NodeAgent agent;
  res.setNodeListener(&agent);
  res.start();
  Thread::sleep2(5000);
  
  


  Thread::sleep2(20000);
  Log::close();
  return (EXIT_SUCCESS);
}

