#include "../ProtocolCommand.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/lang/Exception.h"
#include "../HiveControl.h"

class Status : public ProtocolCommand{
    private:
	Socket * socket;
    public:
	Status(Socket * socket){
	    this->socket=socket;
	    this->is=socket->getInputStream();
	    this->os=socket->getOutputStream();
	}
	Status(InputStream * is, OutputStream * os){
	    this->is=is;
	    this->os=os;
	}
	
	~Status(){}

	int isResponsible (cmdId & cmid) {
	
	}

	int isResponsible(char * command){
	    if(strcmp(command,"show status")==0){
		return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    Properties * status=HiveControl::getInstance()->getStatus();
	    int size=status->toArray().size();
	    string msg="";
	    for(int a=0;a<size;a++){
		msg+=status->toArray()[a].first;
		msg+="\t=\t";
		msg+=status->toArray()[a].second;
		msg+="\n";
	    }
            os->write((char*)msg.c_str(),msg.length());	    
	}

	void printHelp(){
	    string msg="show status\t\t[Status from the server]\n";
            os->write((char*)msg.c_str(),msg.length());	
	}
};
