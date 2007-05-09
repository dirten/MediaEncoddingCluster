#ifndef ORG_ESB_IO_INPUTSTREAM
#define ORG_ESB_IO_INPUTSTREAM
#include <iostream>


namespace org{
namespace esb{
namespace io{
class InputStream {
    public:
	virtual int read(unsigned char * buffer, int length)=0;
	virtual int read(std::string * buffer)=0;
	virtual int available()=0;
	~InputStream(){};
};
}}}
#endif



