#include "../ProtocolCommand.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/lang/Exception.h"
#include "../HiveControl.h"

class StartupHive : public ProtocolCommand{
    private:
//	Socket * socket;
    public:
		/*
	StartupHive(TcpSocket * socket){
	    this->socket=socket;
	    this->is=socket->getInputStream();
	    this->os=socket->getOutputStream();
	}*/
	StartupHive(InputStream * is, OutputStream * os){
	    this->is=is;
	    this->os=os;
	}
	
	~StartupHive(){}
	int isResponsible (cmdId & cmid) {
		return CMD_NA;
	}

	int isResponsible(char * command){
	    if(strcmp(command,"startup")==0){
		return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    string msg="Please wait while HiveControl startup\n";
            os->write((char*)msg.c_str(),msg.length());
	    try{
		HiveControl::getInstance()->startup();
		msg="HiveControl is running\n";
	    }catch(Exception *ex){
		msg=ex->getStackTraceString();
	    }
    	    os->write((char*)msg.c_str(),msg.length());
	}

	void printHelp(){
	    string msg="startup\t\t\t[Startup the server]\n";
            os->write((char*)msg.c_str(),msg.length());	
	}

};
