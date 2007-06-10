#include "Command.h"

using namespace org::esb::hive;
Command::Command(){
    _mode=TEXT_MODE;

}


Command::~Command(){


}
void Command::setCommand(const char * command){
    _command=command;
}
const char * Command::getCommand(){
    return _command;
}
void Command::setData(const void * data){
    _data=data;
}
const void * Command::getData(){
    return _data;
}



