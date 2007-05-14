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
	    string msg="Please Wait while Startup";
            msg+=command;
            msg+="\n";
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());
	    if(HiveControl::getInstance()->startup()){
		msg="Server is running";
        	socket->getOutputStream()->write((char*)msg.c_str(),msg.length());
	    
	    }else{
		msg="Error while startup Server";
        	socket->getOutputStream()->write((char*)msg.c_str(),msg.length());	    
	    }
	    cout << "Server running"<<endl;
	}
};
