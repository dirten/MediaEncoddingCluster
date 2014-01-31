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
#include "org/esb/net/TcpSocket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/db/hivedb.hpp"
#include <list>

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

        delete server;
        server = NULL;
      }

      org::esb::core::OptionsDescription HiveListener::getOptionsDescription() {
        org::esb::core::OptionsDescription result("hiveserver");
        result.add_options()
            ("hiveserver.port", boost::program_options::value<int >()->default_value(20200), "hiveserver port listen on");
        return result;
      }

      org::esb::core::ServicePlugin::ServiceType HiveListener::getServiceType() {
        return org::esb::core::ServicePlugin::SERVICE_TYPE_SERVER;
      }

      void HiveListener::startService() {
        litesql::DataSource<db::ProcessUnit> source = litesql::select<db::ProcessUnit> (*getContext()->database, db::ProcessUnit::Send>1 && db::ProcessUnit::Recv==1);
        litesql::Cursor<db::ProcessUnit> cur=source.cursor();
        for (;cur.rowsLeft(); cur++){
          db::ProcessUnit out=(*cur);
          LOGDEBUG("aborted ProcessUnit found, restart it:"<<out)
          out.send=1;
          out.clientid="";
          out.sendid="";
          out.update();
          LOGDEBUG("changed to:"<<out)
        }


        boost::thread(boost::bind(&HiveListener::startListener, this));
        LOGDEBUG("Hive Listener running on port:" << getContext()->getEnvironment<std::string > ("hiveserver.port"));
        is_running = true;

      }

      void HiveListener::stopService() {
        LOGDEBUG("Hive Listener stopp signal:");
        main_nextloop = false;
        if (server) {
            server->close();
          }
        delete server;
        server = NULL;
        LOGDEBUG("Hive Listener stopped:");

      }

      void HiveListener::startListener() {
        int port = getContext()->getEnvironment<int>("hiveserver.port");
        server = new TcpServerSocket(port);
        server->bind();
        for (;main_nextloop;){
            try {
              TcpSocket * clientSocket = server->accept();
              if (clientSocket && clientSocket->isConnected() && (main_nextloop)) {
                  /*need some client auth and handshake here*/
                  ProtocolServer *protoServer = new ProtocolServer(clientSocket);
                  go(ProtocolServer::run, protoServer);
                } else {
                  LOGERROR("Client was not accepted, maybe server was closed!");
                  //break;
                }
            } catch (exception & ex) {
              LOGERROR("Exception in Main:" <<ex.what());
            }
          }
      }
    }
  }
}

