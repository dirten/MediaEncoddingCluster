#include "../ProtocolCommand.h"
class Disconnect : public ProtocolCommand{
    private:
	Socket * socket;
    public:
	Disconnect(Socket * socket){
	    this->socket=socket;
	}
	
	~Disconnect(){}
	int isResponsible(char * command){
	    if(strcmp(command,"disconnect")==0||
	       strcmp(command,"quit")==0){
		return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}
	void process(char * command){
	    string error="Disconnecting:";
            error+="\n";
            socket->getOutputStream()->write((char*)error.c_str(),error.length());
	    socket->close();
	}

	void printHelp(){
	    string msg="disconnect | quit\t[Disconnecting from the server]\n";
            socket->getOutputStream()->write((char*)msg.c_str(),msg.length());	
	}
};
