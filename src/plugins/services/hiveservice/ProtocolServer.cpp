#include "org/esb/net/TcpSocket.h"
#include "ProtocolServer.h"
#include "org/esb/db/hivedb.hpp"
#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "ProtocolCommand.h"
#include "ProtocolServer.h"
//#include "Command.h"
//#include "CommandInputStream.h"
#include <list>

#include "org/esb/util/Log.h"
/*all Protocols here*/
//#include "protocol/Help.cpp"
//#include "protocol/Kill.cpp"
//#include "proto/AudioDataHandler.cpp"
#include "proto/VideoDataHandler.cpp"
//#include "protocol/Disconnect.cpp"
//#include "protocol/ShowConfig.cpp"
//#include "protocol/ShutdownHive.cpp"
//#include "protocol/StartupHive.cpp"
//#include "protocol/Status.cpp"
//#include "protocol/CreateHive.cpp"
//#include "protocol/Unknown.cpp"
//#include "proto/PartitionHandler.h"


using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;
namespace org {
  namespace esb {
    namespace plugin {

      ProtocolServer::~ProtocolServer() {
        close();
      }

      void ProtocolServer::close() {
        list < ProtocolCommand * >::iterator i;
        for (i = l.begin(); i != l.end(); ++i) {
          ProtocolCommand *tmp = (ProtocolCommand *) * i;
          delete tmp;
        }
        l.clear();
        delete socket;
        socket = NULL;
        //delete _cis;
        //_cis = NULL;
      }

      ProtocolServer::ProtocolServer(TcpSocket * socket) {
        this->socket = socket;
        if (socket == NULL) {
          LOGWARN("Socket seems to be NULL, returning to caller");
          return;
        }
        LOGDEBUG("New Client Arrived from:" << socket->getRemoteIpAddress());
        //_cis = new CommandInputStream(socket->getInputStream());
        //l.push_back(new Help(socket->getInputStream(), socket->getOutputStream()));
        //l.push_back(new AudioDataHandler(socket->getInputStream(), socket->getOutputStream(), socket->getRemoteEndpoint()));
        l.push_back(new VideoDataHandler(socket->getInputStream(), socket->getOutputStream(), socket->getRemoteEndpoint()));
        //l.push_back(new PartitionHandler(socket->getInputStream(), socket->getOutputStream(), socket->getRemoteEndpoint()));
        //l.push_back(new Disconnect(socket->getInputStream(), socket->getOutputStream()));
        //l.push_back(new Kill(socket->getInputStream(), socket->getOutputStream()));
        //l.push_back(new ShowConfig(socket->getInputStream(), socket->getOutputStream()));
        //l.push_back(new ShutdownHive(socket->getInputStream(), socket->getOutputStream()));
        //l.push_back(new StartupHive(socket->getInputStream(), socket->getOutputStream()));
        //l.push_back(new Status(socket->getInputStream(), socket->getOutputStream()));

        //	l.push_back (new Unknown (socket));
      }

      void ProtocolServer::run() {
        LOGINFO("client joined the cluster");
        while (socket->isConnected()) {
          //		logdebug("ProtocolServer::run()::while(!socket->isClosed())")
          //#ifndef DEBUG
          try {
            //#endif
            string cmd;
            int dataLength = socket->getInputStream()->read(cmd);
            if (dataLength <= 0) {
              if (socket->isClosed())
                break;
            }

            char *command = strtok((char *) cmd.c_str(), "\n\r");
            if (command == NULL || strlen(command) <= 0) {
              LOGINFO("Command is NULL, continue!");
              continue;
            }
            LOGDEBUG("Command:"<<command);
            //      logdebug("Command : " << command);
            list < ProtocolCommand * >::iterator i;
            bool handled = false;
            for (i = l.begin(); i != l.end(); ++i) {
              ProtocolCommand *tmp = (ProtocolCommand *) * i;
              if (tmp->isResponsible(command) == CMD_PROCESS) {
                tmp->process(command);
                handled = true;
                break;
              } else if (tmp->isResponsible(command) == CMD_HELP) {
                tmp->printHelp();
                handled = true;
                break;
              }
            }
            if (!handled) {
              LOGERROR("unknown command" << command);
            }
            //#ifndef DEBUG

          } catch (exception & ex) {
            LOGERROR("ERROR in ProtocolServer:" << ex.what());
            //throw ex;
            break;
            //      socket->close();
            //			cout << "ERROR in ProtocolServer:" << ex.what () << endl;
          }
          //#endif
        }
        LOGINFO("client leave the cluster");
        close();
        //  mysql_thread_end();
        //  logdebug("Client Leaved from:"<<socket->getRemoteIpAddress());
        //  	cout << "Elvis has left the Building" << endl;
      }
    }
  }
}
