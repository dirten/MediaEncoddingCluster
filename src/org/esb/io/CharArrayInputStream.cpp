#include "CharArrayInputStream.h"

using namespace org::esb::io;

CharArrayInputStream::~CharArrayInputStream() {
}
CharArrayInputStream::CharArrayInputStream(const char * in, int length) {
    _inPointer=(char *)in;
    _length=length;
}

long long int CharArrayInputStream::available(bool isBlocking){
    return 0;
}

int CharArrayInputStream::read(unsigned char * buffer, int length){
    return 0;
}
int CharArrayInputStream::read(){
    return 0;
}

int CharArrayInputStream::read(vector<unsigned char>&buffer){
    return 0;
}



