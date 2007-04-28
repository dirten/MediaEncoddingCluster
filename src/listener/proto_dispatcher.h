#include <iostream>
#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"

using namespace std;
using namespace org::esb::net;
using namespace org::esb::lang;

class ProtocolServer:public Runnable{
    private:
	Socket * socket;
    public:
	~ProtocolServer();
	ProtocolServer(Socket * socket);
	void run();
};
