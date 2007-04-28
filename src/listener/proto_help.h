#include <iostream>
#include "proto_command.h"
#include "org/esb/net/Socket.h"
using namespace std;
using namespace org::esb::net;


class ProtoHelp : public ProtoCommand{
    private:
	Socket * socket;
    public:
	ProtoHelp(Socket * socket);
	~ProtoHelp();
	bool isResponsible(char * command);
	void process(char * command);
};
