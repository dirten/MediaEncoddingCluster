#ifndef ORG_ESB_AV_CODECOUTPUTSTREAM_H
#define ORG_ESB_AV_CODECOUTPUTSTREAM_H
#include "org/esb/io/OutputStream.h"
#include "Codec.h"
#include <vector>
using namespace org::esb::io;
namespace org{
namespace esb{
namespace av{
class CodecOutputStream: public OutputStream{
    public:
	CodecOutputStream(OutputStream * os);
	void writeCodec(Codec * codec);
	void write(char*buffer, int length);
	void write(vector<unsigned char> & v);
	void flush();
    private:
	OutputStream * _os;





};
}}}
#endif

