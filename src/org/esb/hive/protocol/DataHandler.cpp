#include "../ProtocolCommand.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "../job/Job.h"
#include "../job/JobHandler.h"

using namespace org::esb::hive::job;
using namespace org::esb::av;
class DataHandler: public ProtocolCommand{
    private:
	InputStream * _is;
	OutputStream * _os;
    public:
	DataHandler(InputStream * is, OutputStream * os){
	    _is=is;
	    _os=os;
	}

	int isResponsible(char * command){
	    if(strcmp(command,"get frame")==0||
	       strcmp(command,"put frame")==0){
		    return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    Job * job=JobHandler::getInstance()->getJob();
	    if(strcmp(command,"get frame")==0){
		PacketOutputStream pos(_os);
		Packet packet=job->getPacket();
//		cout << "PacketSize"<<packet.getSize();
		pos.writePacket(&job->getPacket());
//		_os->write((char*)t.c_str(), t.size());
	    }else
	    if(strcmp(command,"put frame")==0){
		string t="getting frame";
		_os->write((char*)t.c_str(), t.size());	    
	    }
	}

	void printHelp(){
	}
};
