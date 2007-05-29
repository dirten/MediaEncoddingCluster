#ifndef ORG_ESB_IO_INPUTSTREAM
#define ORG_ESB_IO_INPUTSTREAM
#include <iostream>


namespace org {
    namespace esb {
        namespace io {
            class InputStream {
public:
                virtual int read(unsigned char * buffer, int length)=0;
                virtual int available(bool isBlocking=false)=0;
                virtual void close() {}
                ;
                virtual ~InputStream() {}
                ;
            };
        }
    }
}
#endif



