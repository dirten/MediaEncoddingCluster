#include "../ProtocolCommand.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/CodecOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/av/Packet.h"
#include "../job/Job.h"
#include "../job/JobHandler.h"
#include "../job/ClientHandler.h"
#include "../job/ProcessUnit.h"

using namespace org::esb::hive::job;
using namespace org::esb::av;
using namespace org::esb;


#define GET_UNIT  "get process_unit"
#define PUT_UNIT  "put process_unit"

class DataHandler: public ProtocolCommand{
    private:
	InputStream * _is;
	OutputStream * _os;
//	PacketOutputStream * _pos;
	io::ObjectOutputStream * _oos;
	io::ObjectInputStream * _ois;
	ClientHandler* _handler;

    public:
	    DataHandler(InputStream * is, OutputStream * os){
	    _is=is;
	    _os=os;
//	    _pos=new PacketOutputStream(_os);
	    _oos=new io::ObjectOutputStream(_os);
	    _ois=new io::ObjectInputStream(_is);
	    _handler=new ClientHandler();
	}
	int isResponsible (cmdId & cmid) {
	
	}

	int isResponsible(char * command){
	    if(
	       strcmp(command,GET_UNIT)==0||
	       strcmp(command,PUT_UNIT)==0){
		    return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
			return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    if(strcmp(command,GET_UNIT)==0){
		ProcessUnit  un=_handler->getProcessUnit();
		_oos->writeObject(un);
	    }else
	    if(strcmp(command,PUT_UNIT)==0){
		ProcessUnit un;
	    	_ois->readObject(un);
		if(!_handler->putProcessUnit(un)){
		    cout << "error while putProcessUnit!";
		}
	    }
	}
	void printHelp(){
	}
};
