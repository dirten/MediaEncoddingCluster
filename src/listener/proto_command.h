#ifndef LISTENER_PROTO_COMMAND_H
#define LISTENER_PROTO_COMMAND_H
#include <iostream>

using namespace std;

class ProtoCommand{
    public:
	virtual ~ProtoCommand(){};
	virtual bool isResponsible(char * command)=0;
	virtual void process(char * command)=0;
};

#endif
