#ifndef ORG_ESB_IO_CHARARRAYINPUTSTREAM_H
#define ORG_ESB_IO_CHARARRAYINPUTSTREAM_H
#include "InputStream.h"
#include <vector>
#include "org/esb/io/exports.h"
namespace org {
    namespace esb {
        namespace io {
class  CharArrayInputStream: public InputStream {
public:
                CharArrayInputStream(const char * in, int length);
                ~CharArrayInputStream();
		long long int available(bool isBlocking=false);
		int read(unsigned char * buffer, int length);
		int read(vector<unsigned char>&buffer);
		int read();
private:
                char * _inPointer;
                long _length;
            };
        }
    }
  }

#endif
