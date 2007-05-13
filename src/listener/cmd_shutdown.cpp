#include "proto_command.h"
#include "org/esb/lang/Thread.h"
#include "hivecontrol/hivecontrol.h"

class ProtoShutdown : public ProtoCommand{
    private:
	Socket * socket;
    public:
	ProtoShutdown(Socket * socket){
	    this->socket=socket;
	}
	
	~ProtoShutdown(){}
	bool isResponsible(char * command){
	    if(strcmp(command,"shutdown")==0){
		return true;
	    }
	    return false;
	}
	void process(char * command){
	    string error="Please Wait while Shutdown";
            error+=command;
            error+="\n";
            socket->getOutputStream()->write((char*)error.c_str(),error.length());
	    HiveControl::getInstance()->shutdown();
	    cout << "Server is stopped"<<endl;

//            delete error;
	}
};
