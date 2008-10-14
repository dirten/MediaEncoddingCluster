#include <iostream>
#include "../ProtocolCommand.h"


using namespace std;
using namespace org::esb::net;


class Unknown : public ProtocolCommand{
    private:
//	Socket * socket;
    public:
	Unknown(TcpSocket * socket){
	    this->socket=socket;
	    this->is=socket->getInputStream();
	    this->os=socket->getOutputStream();
	}
	Unknown(InputStream * is, OutputStream * os){
	    this->is=is;
	    this->os=os;
	}
	
	~Unknown(){}
	int isResponsible (cmdId & cmid) {
		return CMD_NA;
	}

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
            os->write((char*)error.c_str(),error.length());
//            delete error;
	}
};
