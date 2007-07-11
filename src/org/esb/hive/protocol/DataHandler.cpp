#include "../ProtocolCommand.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/CodecOutputStream.h"
#include "org/esb/av/Packet.h"
#include "../job/Job.h"
#include "../job/JobHandler.h"

using namespace org::esb::hive::job;
using namespace org::esb::av;

class DataHandler: public ProtocolCommand{
    private:
	InputStream * _is;
	OutputStream * _os;
	PacketOutputStream * _pos;
	Job * _job;
	static pthread_mutex_t * mutex;;

    public:
	DataHandler(InputStream * is, OutputStream * os){
	    _is=is;
	    _os=os;
	    _pos=new PacketOutputStream(_os);
	    _job=JobHandler::getInstance()->getJob();
	    if(mutex==0){
	    cout << "Mutex Create"<<endl;
	    mutex=(pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	    pthread_mutexattr_t attr;
	    pthread_mutexattr_init(&attr);
	    pthread_mutex_init(mutex, &attr);
	    pthread_mutexattr_destroy(&attr);
	    }

	}

	int isResponsible(char * command){
	    if(strcmp(command,"get frame")==0||
	       strcmp(command,"put frame")==0||
	       strcmp(command,"get inputcodec")==0||
	       strcmp(command,"get outputcodec")==0){
		    return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * command){
	    if(strcmp(command,"get frame")==0){
			pthread_mutex_lock(mutex);
			Packet packet=_job->getPacket();
			Packet *tmp=new Packet(&packet);
			pthread_mutex_unlock(mutex);
//			packet=_job->getPacket();
			_pos->writePacket(tmp);
//			_pos->writePacket(&packet);
			delete tmp;

	    }else
	    if(strcmp(command,"put frame")==0){
			string t="getting frame";
			_os->write((char*)t.c_str(), t.size());	    
	    }else
	    if(strcmp(command,"get inputcodec")==0){
			CodecOutputStream cos(_os);
			cos.writeCodec(_job->getInputCodec());
	    }else
	    if(strcmp(command,"get outputcodec")==0){
			CodecOutputStream cos(_os);
			cos.writeCodec(_job->getOutputCodec());
	    }
	}

	void printHelp(){
	}
};
pthread_mutex_t * DataHandler::mutex=0;
