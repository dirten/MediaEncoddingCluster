#include "../ProtocolCommand.h"
class Disconnect:public ProtocolCommand {
  public:
/*
	Disconnect (TcpSocket * socket) {
		this->socket = socket;
		this->is = socket->getInputStream ();
		this->os = socket->getOutputStream ();
	} 
*/	
	Disconnect (InputStream * is, OutputStream * os) {

		this->is = is;
		this->os = os;
	}

	~Disconnect () {
	}

	int isResponsible (cmdId & cmid) {
		return CMD_NA;
	}
	int isResponsible (char *command) {
		if (strcmp (command, "disconnect") == 0 ||
			strcmp (command, "quit") == 0) {
			return CMD_PROCESS;
		}
		else if (strcmp (command, "help") == 0) {
			return CMD_HELP;
		}
		return CMD_NA;
	}
	void process (char *command) {
		string error = "Disconnecting:";
		error += "\n";
		os->write ((char *) error.c_str (), error.length ());
	}

	void printHelp () {
		string msg = "disconnect | quit\t[Disconnecting from the server]\n";
		os->write ((char *) msg.c_str (), msg.length ());
	}
};
