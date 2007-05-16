#ifndef LISTENER_PROTO_COMMAND_H
#define LISTENER_PROTO_COMMAND_H
#include <iostream>
#define CMD_NA 0
#define CMD_PROCESS 1
#define CMD_HELP 2
using namespace std;

class ProtoCommand{
    public:
	virtual ~ProtoCommand(){};
	virtual int isResponsible(char * command)=0;
	virtual void process(char * command)=0;
	virtual void printHelp(){};
};

#endif
