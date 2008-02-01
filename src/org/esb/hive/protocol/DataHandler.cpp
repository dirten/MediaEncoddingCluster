#include "../ProtocolCommand.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/CodecOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/av/Packet.h"
#include "../job/Job.h"
#include "../job/JobHandler.h"
#include "../job/ClientHandler.h"
#include "../job/ProcessUnit.h"

using namespace org::esb::hive::job;
using namespace org::esb::av;
using namespace org::esb;

class DataHandler: public ProtocolCommand{
    private:
	InputStream * _is;
	OutputStream * _os;
	PacketOutputStream * _pos;
	io::ObjectOutputStream * _oos;
	ClientHandler* _handler;

    public:
	    DataHandler(InputStream * is, OutputStream * os){
	    _is=is;
	    _os=os;
	    _pos=new PacketOutputStream(_os);
	    _oos=new io::ObjectOutputStream(_os);
	    _handler=new ClientHandler();
	}

	int isResponsible(char * command){
	    if(strcmp(command,"get frame")==0||
	       strcmp(command,"put frame")==0||
	       strcmp(command,"get inputcodec")==0||
	       strcmp(command,"get outputcodec")==0||
	       strcmp(command,"get process_unit")==0||
	       strcmp(command,"put process_unit")==0){
		    return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    if(strcmp(command,"get frame")==0){
	    }else
	    if(strcmp(command,"put frame")==0){
			string t="getting frame";
			_os->write((char*)t.c_str(), t.size());
	    }else
	    if(strcmp(command,"get inputcodec")==0){
	    }else
	    if(strcmp(command,"get outputcodec")==0){
	    }else
	    if(strcmp(command,"get process_unit")==0){
	    	ProcessUnit * unit=_handler->getProcessUnit();
	    	_oos->writeObject(*unit);
	    }else
	    if(strcmp(command,"put process_unit")==0){
	    }
	}
	void printHelp(){
	}
};
