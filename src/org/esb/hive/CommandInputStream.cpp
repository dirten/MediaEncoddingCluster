#include "CommandInputStream.h"
#include "Command.h"
#include <iostream>

using namespace std;
using namespace org::esb::hive;


CommandInputStream::CommandInputStream(InputStream * is){
    _source=is;

}


CommandInputStream::~CommandInputStream(){

}

Command * CommandInputStream::readCommand(){
    unsigned char buffer;
	string b;
    while(read(&buffer,1)>0){
		b+=buffer;
    }
	Command * command=new Command();
	char * com=new char[b.length()+1];
	memset(com,0,b.length()+1);
	memcpy(com, b.c_str(), b.length());
	command->setCommand(com);
	
    return command;
}


int CommandInputStream::read(unsigned char * buffer, int length){
    return _source->read(buffer,length);
}


int CommandInputStream::available(bool isBlocking){
    _source->available(isBlocking);
}
