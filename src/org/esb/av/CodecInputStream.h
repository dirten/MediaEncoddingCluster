#ifndef ORG_ESB_AV_CODECINPUTSTREAM_H
#define ORG_ESB_AV_CODECINPUTSTREAM_H
#include "org/esb/io/InputStream.h"
#include "Codec.h"
using namespace org::esb::io;
namespace org{
namespace esb{
namespace av{
class CodecInputStream: public InputStream{
    public:
	CodecInputStream(InputStream * is);
	~CodecInputStream();
	Codec * readCodec();
	int read(unsigned char * buffer, int length);
	int read(vector<unsigned char>&buffer);
	int read();
	int available(bool isBlocking=false);
    private:
	InputStream * _is;
	Codec * _codec;

};
}}}
#endif

