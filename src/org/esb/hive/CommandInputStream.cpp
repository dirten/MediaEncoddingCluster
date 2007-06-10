#include "CommandInputStream.h"
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
    while(read(&buffer,1)>0){
        cout << buffer;
        fflush(NULL);
    }
    cout << endl;
    
}


int CommandInputStream::read(unsigned char * buffer, int length){
    return _source->read(buffer,length);
}


int CommandInputStream::available(bool isBlocking){
    _source->available(isBlocking);
}
