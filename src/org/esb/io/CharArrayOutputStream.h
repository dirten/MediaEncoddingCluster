#ifndef ORG_ESB_IO_CHARARRAYOUTPUTSTREAM
#define ORG_ESB_IO_CHARARRAYOUTPUTSTREAM
#include "OutputStream.h"
#include <vector>
#include "org/esb/io/exports.h"
namespace org{
namespace esb{
namespace io{
class  CharArrayOutputStream: public OutputStream{
    public:
	CharArrayOutputStream();
	~CharArrayOutputStream();
	void write(char * buffer, int length);
	void write(vector<unsigned char>&buffer);

      };
}}}
#endif
