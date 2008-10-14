#include "../ProtocolCommand.h"
#include "org/esb/lang/Thread.h"
#include "../HiveControl.h"
using namespace org::esb::hive;
class ShutdownHive : public ProtocolCommand{
    private:
//	TcpSocket * socket;
    public:
	ShutdownHive(TcpSocket * socket){
	    this->socket=socket;
	    this->is=socket->getInputStream();
	    this->os=socket->getOutputStream();
	}
	ShutdownHive(InputStream * is, OutputStream * os){
	    this->is=is;
	    this->os=os;
	}
	

	~ShutdownHive(){}
	int isResponsible (cmdId & cmid) {
		return CMD_NA;
	}

	int isResponsible(char * command){
	    if(strcmp(command,"shutdown")==0){
		return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    string msg="Please wait while stopping HiveControl\n";
            os->write((char*)msg.c_str(),msg.length());
	    try{
		HiveControl::getInstance()->shutdown();
		msg="HiveControl shutdown\n";
	    }catch(Exception *ex){
		msg=ex->getStackTraceString();
	    }
    	os->write((char*)msg.c_str(),msg.length());
	}

	void printHelp(){
	    string msg="shutdown\t\t[Stopps the server]\n";
            os->write((char*)msg.c_str(),msg.length());	
	}

};
