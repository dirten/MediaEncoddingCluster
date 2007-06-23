#include "CharArrayOutputStream.h"
using namespace org::esb::io;
CharArrayOutputStream::CharArrayOutputStream(){

}
CharArrayOutputStream::~CharArrayOutputStream(){

}
/******************************************************************************/
void CharArrayOutputStream::write(vector<unsigned char>&buffer){
    write((char*)&buffer[0], buffer.size());
}

void CharArrayOutputStream::write(char * buffer, int length){

}
