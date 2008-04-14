#include "ProtocolServer.h"
#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "ProtocolCommand.h"
#include "ProtocolServer.h"
#include "Command.h"
#include "CommandInputStream.h"
#include <list>

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




using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;
using namespace org::esb::util;
using namespace org::esb::hive;

ProtocolServer::~ProtocolServer ()
{
	list < ProtocolCommand * >::iterator i;
	for (i = l.begin (); i != l.end (); ++i) {
		ProtocolCommand *tmp = (ProtocolCommand *) * i;
		delete tmp;
	}
	l.clear ();
	delete socket;
	socket = 0;
	delete _cis;
}

ProtocolServer::ProtocolServer (Socket * socket)
{
	this->socket = socket;
	_cis = new CommandInputStream (socket->getInputStream ());
	l.push_back (new
				 Help (socket->getInputStream (),
					   socket->getOutputStream ()));
	l.push_back (new
				 DataHandler (socket->getInputStream (),
							  socket->getOutputStream ()));
	l.push_back (new Disconnect (socket));
	l.push_back (new Kill (socket));
	l.push_back (new ShowConfig (socket));
	l.push_back (new ShutdownHive (socket));
	l.push_back (new StartupHive (socket));
	l.push_back (new Status (socket));
	l.push_back (new Unknown (socket));
}

void ProtocolServer::run ()
{
	while (!socket->isClosed ()) {
		try {
			string cmd;
			int dataLength = socket->getInputStream ()->read (cmd);
			if (dataLength == 0) {
				if (socket->isClosed ())
					break;
			}
			char *command = strtok ((char *) cmd.c_str (), "\n\r");
			if (command == NULL || strlen (command) <= 0)
				continue;
			list < ProtocolCommand * >::iterator i;
			for (i = l.begin (); i != l.end (); ++i) {
				ProtocolCommand *tmp = (ProtocolCommand *) * i;
				if (tmp->isResponsible (command) == CMD_PROCESS) {
					tmp->process (command);
					break;
				}
				else if (tmp->isResponsible (command) == CMD_HELP) {
					tmp->printHelp ();
				}
			}
		}
		catch (exception & ex) {
			logerror("ERROR in ProtocolServer:" << ex.what ());
//			cout << "ERROR in ProtocolServer:" << ex.what () << endl;
		}
	}
	cout << "Elvis has left the Building" << endl;
}
