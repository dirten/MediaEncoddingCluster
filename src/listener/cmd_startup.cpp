#include "proto_command.h"
#include "org/esb/lang/Thread.h"
#include "hivecontrol/hivecontrol.h"

class ProtoStartup : public ProtoCommand{
    private:
	Socket * socket;
    public:
	ProtoStartup(Socket * socket){
	    this->socket=socket;
	}
	
	~ProtoStartup(){}
	bool isResponsible(char * command){
	    if(strcmp(command,"startup")==0){
		return true;
	    }
	    return false;
	}
	void process(char * command){
	    string error="Please Wait while Startup";
            error+=command;
            error+="\n";
            socket->getOutputStream()->write((char*)error.c_str(),error.length());
	    HiveControl::getInstance()->startup();
	    cout << "Server running"<<endl;

//            delete error;
	}
};
