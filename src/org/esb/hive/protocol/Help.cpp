#ifndef LISTENER_PROTO_HELP
#define LISTENER_PROTO_HELP
#include "../ProtocolCommand.h"


using namespace org::esb::net;
using namespace org::esb::hive;
using namespace std;

class Help : public ProtocolCommand {
private:
  TcpSocket * socket;
public:

  ~Help() {
  }

  Help(TcpSocket * socket) {
    this->socket = socket;
    this->is = socket->getInputStream();
    this->os = socket->getOutputStream();
  }

  Help(InputStream * is, OutputStream * os) {
    this->is = is;
    this->os = os;
  }

  int isResponsible(cmdId & cmid) {
    return CMD_NA;
  }

  int isResponsible(char *command) {
    if (strcmp(command, "help") == 0)
      return CMD_HELP;
    return CMD_NA;
  }

  void process(char *data) {
  }

  void printHelp() {
    string help = "List of all Hive Commands\n";
    help += "--------------------------------------------------------\n";
    os->write((char *) help.c_str(), help.length());
  }
};
#endif
