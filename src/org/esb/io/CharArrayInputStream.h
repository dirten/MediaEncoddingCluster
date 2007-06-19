#ifndef ORG_ESB_IO_CHARARRAYINPUTSTREAM_H
#define ORG_ESB_IO_CHARARRAYINPUTSTREAM_H
#include "InputStream.h"
namespace org {
    namespace esb {
        namespace io {
class CharArrayInputStream: public InputStream {
public:
                CharArrayInputStream(const char * in, long length);
                ~CharArrayInputStream();

private:
                char * _inPointer;
                long _length;
            };
        }
    }
}
#endif

