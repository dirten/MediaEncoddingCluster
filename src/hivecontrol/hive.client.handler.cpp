#include "org/esb/lang/Runnable.h"
#include "org/esb/net/Socket.h"

using namespace org::esb::lang;
using namespace org::esb::net;

class HiveClientHandler: public Runnable{
    private:
	Socket * socket;
    public:
	HiveClientHandler(Socket * socket){
	    this->socket=socket;
	}
	
	~HiveClientHandler(){
	    delete socket;
	    socket=0;
	}
	
	void run(){
	    
	
	}
};
