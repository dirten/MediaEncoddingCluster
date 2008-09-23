#include "../ProtocolCommand.h"
class Disconnect:public ProtocolCommand {
  public:

	Disconnect (Socket * socket) {
		this->socket = socket;
		this->is = socket->getInputStream ();
		this->os = socket->getOutputStream ();
	} 
	
	Disconnect (InputStream * is, OutputStream * os) {
		this->socket = 0;
		this->is = is;
		this->os = os;
	}

	~Disconnect () {
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
		if (socket)
			socket->close ();
	}

	void printHelp () {
		string msg = "disconnect | quit\t[Disconnecting from the server]\n";
		os->write ((char *) msg.c_str (), msg.length ());
	}
};
