#include "CharArrayInputStream.h"

using namespace org::esb::io;
CharArrayInputStream::CharArrayInputStream(const char * in, long length) {
    _inPointer=(char *)in;
    _length=length;
}


