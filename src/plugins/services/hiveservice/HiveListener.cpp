/*----------------------------------------------------------------------
 *  File    : HiveListener.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose : Main Client connection Server
 *  Created : 18 Feb 2009 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */
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
    namespace plugin {

      HiveListener::HiveListener() {
        server = NULL;
        main_nextloop = true;
        is_running = false;
      }

      HiveListener::~HiveListener() {
        //        server->close();
      }

      void HiveListener::onMessage(org::esb::signal::Message & msg) {
        if (msg.getProperty("hivelistener") == org::esb::hive::START) {
          //    cout << "Start Message Arrived:"<<endl;
          boost::thread tt(boost::bind(&HiveListener::startListener, this));
          LOGDEBUG("Hive Listener running on port:" << Config::get("hive.port"));
          //    cout << "Hive Listener running:"<<endl;
          is_running = true;
        } else
          if (msg.getProperty("hivelistener") == org::esb::hive::STOP) {
          LOGDEBUG( "Hive Listener stopp signal:");
          main_nextloop = false;
          if (server){
            
            server->close();
          }
          delete server;
          server = NULL;
          LOGDEBUG( "Hive Listener stopped:");
          //    cout << "Stop Message Arrived:"<<endl;
        }
      }
        void HiveListener::startService(){
          boost::thread tt(boost::bind(&HiveListener::startListener, this));
          LOGDEBUG("Hive Listener running on port:" << Config::get("hive.port"));
          is_running = true;
          
        }
        void HiveListener::stopService(){
          LOGDEBUG( "Hive Listener stopp signal:");
          main_nextloop = false;
          if (server){            
            server->close();
          }
          delete server;
          server = NULL;
          LOGDEBUG( "Hive Listener stopped:");
          
        }

      void HiveListener::startListener() {

        int port = atoi(Config::get("hive.port").c_str());
        LOGDEBUG(" Start Listening on port "<<port);
        server = new TcpServerSocket(port);
        server->bind();
        for (; main_nextloop;) {
          //          try {
          TcpSocket * clientSocket = server->accept();
          if (clientSocket&&clientSocket->isConnected() &&(main_nextloop)) {
            /*need some client auth and handshake here*/
            ProtocolServer *protoServer = new ProtocolServer(clientSocket);
            go(ProtocolServer::run, protoServer);
            
            //boost::thread tt(boost::bind(&ProtocolServer::run, protoServer));
            //              Thread *thread = new Thread(protoServer);
            //              thread->start();
          } else {
            LOGERROR("Client was not accepted");
            break;
          }
          //          } catch (exception & ex) {
          //            logerror("Exception in Main:" <<ex.what());
          //          }
        }
      }

    }
  }
}

