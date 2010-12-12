/*
 * File:   HiveClientAudio.cpp
 * Author: jholscher
 *
 * Created on 12. Dezember 2010, 00:11
 */

#include <cstdlib>
#include "org/esb/hive/NodeResolver.h"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/signal/Message.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Messenger.h"
#include "config.h"
using namespace std;
using namespace org::esb::hive;
using namespace org::esb::signal;
using namespace org::esb::lang;
class NodeAgent : public NodeListener {

  classlogger("NodeListener")
public:
  NodeAgent() {
    having_server = false;
  }

  void onNodeUp(Node & node) {
    LOGDEBUG("NodeUp:" << node.toString());
    LOGDEBUG("NodeData " << node.getData("type"));
    if (node.getData("type") == "server" && !having_server) {
      string host = node.getIpAddress().to_string();
      int port = atoi(node.getData("port").c_str());
      LOGINFO("Server Node:" << node.toString());
      /**
       * @TODO: this is a memleak here, the created objects must be deleted
       *
       */

      _client=Ptr<org::esb::hive::HiveClient>(new org::esb::hive::HiveClient(host, port));
      Messenger::getInstance().addMessageListener(*_client.get());

      Messenger::getInstance().sendMessage(Message().setProperty("hiveclient", org::esb::hive::START));
      having_server = true;
      /*
       org::esb::hive::HiveClient client(host, port);
       client.start();
       */
    }
  }

  void onNodeDown(Node & node) {
    LOGWARN("NodeDown:" << node.toString());
    if (node.getData("type") == "server") {
      LOGWARN("ServerNode is Down, let the client sleep!:" << node.toString());

       Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));
       _client.reset();
       Messenger::free();
       having_server=false;
    }
  }
private:
  bool having_server;
  Ptr<org::esb::hive::HiveClient> _client;
};

/*
 *
 */
int main(int argc, char** argv) {
  Log::open();
//  org::esb::lang::Thread::sleep2(3000);
  org::esb::hive::Node node;
  node.setData("type", "client");
  node.setData("version", MHIVE_VERSION);
  org::esb::hive::NodeResolver res(boost::asio::ip::address::from_string("0.0.0.0"), boost::asio::ip::address::from_string("239.255.0.1"), 6000, node);
  NodeAgent agent;
  res.setNodeListener(&agent);
  res.start();
  org::esb::lang::CtrlCHitWaiter::wait();

  Messenger::getInstance().sendRequest(Message().setProperty("hiveclient", org::esb::hive::STOP));
  Messenger::free();

  return 0;
}

