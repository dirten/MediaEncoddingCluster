#include <iostream>
#include "../ProtocolCommand.h"
#include "org/esb/net/Socket.h"

using namespace std;
using namespace org::esb::net;


class Unknown : public ProtocolCommand{
    private:
	Socket * socket;
    public:
	Unknown(Socket * socket){
	    this->socket=socket;
	}
	
	~Unknown(){}
	int isResponsible(char * command){
	    if(strlen(command)>0&&strcmp(command,"help")!=0){
//		cout <<"Command Unknown:"<<command<<endl;
		return CMD_PROCESS;
	    }
	    return CMD_NA;
	}
	void process(char * command){
	    string error="Unknown Command:";
            error+=command;
            error+="\n";
            socket->getOutputStream()->write((char*)error.c_str(),error.length());
//            delete error;
	}
};
