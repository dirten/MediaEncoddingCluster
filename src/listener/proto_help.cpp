#include "proto_help.h"

using namespace org::esb::net;

	ProtoHelp::~ProtoHelp(){
	}

	ProtoHelp::ProtoHelp(Socket * socket){
	    this->socket=socket;
	}

	bool ProtoHelp::isResponsible(char * command){
	    return (strcmp(command,"help")==0);
	}

	void ProtoHelp::process(char * data){
	    string help="Using help from CommandLine";
	    socket->write((unsigned char *)help.c_str(),help.length());
	}

