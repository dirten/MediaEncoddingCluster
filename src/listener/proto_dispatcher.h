#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"
#include "org/esb/util/StringUtil.h"
#include "proto_help.h"
#include <list>

using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;

class ProtocolServer:public Runnable{
    private:
	Socket * socket;
	list<ProtoCommand*>  l;
    public:
	~ProtocolServer();
	ProtocolServer(Socket * socket);
	void run();
};
