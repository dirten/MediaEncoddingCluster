#include "../ProtocolCommand.h"
//#include <sqlite3.h>



using namespace org::esb::net;
using namespace org::esb::hive;
using namespace std;


class CreateHive : public ProtocolCommand{

    private:
		Socket * socket;
    public:
		~CreateHive(){
	}
    
	CreateHive(Socket * socket){
	    this->socket=socket;
	    this->is=socket->getInputStream();
	    this->os=socket->getOutputStream();
	}
	CreateHive(InputStream * is, OutputStream * os){
	    this->is=is;
	    this->os=os;
	}

	int isResponsible(char * command){
	    if(strstr(command,"create hive")>0){
		    return CMD_PROCESS;
	    }else
	    if(strcmp(command,"help")==0){
		    return CMD_HELP;
	    }
	    return CMD_NA;
	}

	void process(char * data){
		string msg="Creating Hive at ";
		string path=data+12;
		msg+=path;
		os->write((char*)msg.c_str(),msg.length());
		string tableFile="CREATE TABLE FILE (id, name, size, type)";
		string tablePacket="CREATE TABLE PACKET (id,pts,dts,stream_index,flags,duration,pos,data_size,data)";
		string tableJob="CREATE TABLE JOB(id,infile,outfile)";
    	sqlite3 *db;
    	sqlite3_stmt *pStmt;
    	char *zErrMsg = 0;
    	int rc = sqlite3_open(path.c_str(), &db);
    	if( rc ){
        	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        	sqlite3_close(db);
        	return;
    	}
    	sqlite3_exec(db,tableFile.c_str(),NULL,NULL,NULL);
    	sqlite3_exec(db,tablePacket.c_str(),NULL,NULL,NULL);
    	sqlite3_exec(db,tableJob.c_str(),NULL,NULL,NULL);
	}

	void printHelp(){
	}

};
