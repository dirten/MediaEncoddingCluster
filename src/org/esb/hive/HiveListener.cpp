#include "HiveListener.h"
#include "ProtocolServer.h"
#include "org/esb/config/config.h"
#include "org/esb/net/TcpSocket.h"
//#include "org/esb/net/TcpServerSocket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/signal/Messenger.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
//#include "org/esb/lang/Runnable.h"
#include <list>
using namespace org::esb::config;
using namespace org::esb::lang;
using namespace org::esb::net;

namespace org {
  namespace esb {
    namespace hive {

      HiveListener::HiveListener() {
        server = NULL;
        main_nextloop = true;
        is_running = false;
      }

      HiveListener::~HiveListener() {
//        server->close();
      }

      void HiveListener::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("hivelistener") == "start") {
          //    cout << "Start Message Arrived:"<<endl;
          boost::thread tt(boost::bind(&HiveListener::startListener, this));
          logdebug("Hive Listener running on port:" << Config::getProperty("hive.port","20200"));
          //    cout << "Hive Listener running:"<<endl;
          is_running = true;
        } else
          if (msg.getProperty("hivelistener") == "stop") {
          logdebug("Hive Listener stopped:");
          main_nextloop = false;
          if (server)
            server->close();
          delete server;
          server = NULL;
          //    cout << "Stop Message Arrived:"<<endl;
        }
      }

      void HiveListener::startListener() {
        int port = atoi(Config::getProperty("hive.port","20200"));
        server = new TcpServerSocket(port);
        server->bind();
        for (; main_nextloop;) {
          try {
            TcpSocket * clientSocket = server->accept();
            logdebug("new CLient here");
            if (clientSocket != NULL) {
              ProtocolServer *protoServer = new ProtocolServer(clientSocket);
              Thread *thread = new Thread(protoServer);
              thread->start();
            } else {
              logerror("Client  Socket ist null");
              break;
            }
          } catch (exception & ex) {
            logerror("Exception in Main:" << ex.what());
          }
        }
      }

    }
  }
}

