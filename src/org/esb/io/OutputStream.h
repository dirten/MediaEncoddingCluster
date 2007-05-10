#ifndef ORG_ESB_IO_OUTPUTSTREAM
#define ORG_ESB_IO_OUTPUTSTREAM


namespace org{
namespace esb{
namespace io{
class OutputStream {
    public:
	virtual void write(char * buffer, int length)=0;
	~OutputStream(){};
};
}}}
#endif

