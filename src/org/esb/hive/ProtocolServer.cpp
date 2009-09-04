#include "ProtocolServer.h"
#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/TcpSocket.h"
#include "ProtocolCommand.h"
#include "ProtocolServer.h"
#include "Command.h"
#include "CommandInputStream.h"
#include <list>

#include "org/esb/util/Log.h"
/*all Protocols here*/
#include "protocol/Help.cpp"
#include "protocol/Kill.cpp"
#include "protocol/DataHandler.cpp"
#include "protocol/Disconnect.cpp"
#include "protocol/ShowConfig.cpp"
#include "protocol/ShutdownHive.cpp"
#include "protocol/StartupHive.cpp"
#include "protocol/Status.cpp"
//#include "protocol/CreateHive.cpp"
#include "protocol/Unknown.cpp"


#define DEBUG
#undef DEBUG
using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;
using namespace org::esb::hive;

ProtocolServer::~ProtocolServer() {
  list < ProtocolCommand * >::iterator i;
  for (i = l.begin(); i != l.end(); ++i) {
    ProtocolCommand *tmp = (ProtocolCommand *) * i;
    delete tmp;
  }
  l.clear();
  delete socket;
  socket = 0;
  delete _cis;
}

ProtocolServer::ProtocolServer(TcpSocket * socket) {
  this->socket = socket;
  logdebug("New Client Arrived from:"<<socket->getRemoteIpAddress());
  _cis = new CommandInputStream(socket->getInputStream());
  l.push_back(new Help(socket->getInputStream(), socket->getOutputStream()));
  l.push_back(new DataHandler(socket->getInputStream(), socket->getOutputStream()));
  l.push_back(new Disconnect(socket->getInputStream(), socket->getOutputStream()));
  l.push_back(new Kill(socket->getInputStream(), socket->getOutputStream()));
  l.push_back(new ShowConfig(socket->getInputStream(), socket->getOutputStream()));
  l.push_back(new ShutdownHive(socket->getInputStream(), socket->getOutputStream()));
  l.push_back(new StartupHive(socket->getInputStream(), socket->getOutputStream()));
  l.push_back(new Status(socket->getInputStream(), socket->getOutputStream()));

  //	l.push_back (new Unknown (socket));
}

void ProtocolServer::run() {
	while (!socket->isClosed()) {
//		logdebug("ProtocolServer::run()::while(!socket->isClosed())")
#ifndef DEBUG
	  try {
#endif
		  string cmd;
      int dataLength = socket->getInputStream()->read(cmd);
      if (dataLength == 0) {
        if (socket->isClosed())
          break;
      }
      
      char *command = strtok((char *) cmd.c_str(), "\n\r");
      if (command == NULL || strlen(command) <= 0) {
        logdebug("Command is NULL, continue!");
        continue;
      }
//      logdebug("Command : " << command);
      list < ProtocolCommand * >::iterator i;
      for (i = l.begin(); i != l.end(); ++i) {
        ProtocolCommand *tmp = (ProtocolCommand *) * i;
        if (tmp->isResponsible(command) == CMD_PROCESS) {
          tmp->process(command);
          break;
        } else if (tmp->isResponsible(command) == CMD_HELP) {
          tmp->printHelp();
        }
      }
#ifndef DEBUG
    } catch (exception & ex) {
      logerror("ERROR in ProtocolServer:" << ex.what());
      socket->close();
      //			cout << "ERROR in ProtocolServer:" << ex.what () << endl;
    }
#endif
  }
//  logdebug("Client Leaved from:"<<socket->getRemoteIpAddress());
//  	cout << "Elvis has left the Building" << endl;
}
