#ifndef ORG_ESB_IO_CHARARRAYINPUTSTREAM_H
#define ORG_ESB_IO_CHARARRAYINPUTSTREAM_H
#include "InputStream.h"
#include <vector>
namespace org {
    namespace esb {
        namespace io {
class CharArrayInputStream: public InputStream {
public:
                CharArrayInputStream(const char * in, int length);
                ~CharArrayInputStream();
		int available(bool isBlocking=false);
		int read(unsigned char * buffer, int length);
		int read(vector<unsigned char>&buffer);
private:
                char * _inPointer;
                long _length;
            };
        }
    }
  }

#endif
