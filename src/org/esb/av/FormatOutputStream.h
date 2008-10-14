#ifndef ORG_ESB_AV_FORMATOUTPUTSTREAM_H
#define ORG_ESB_AV_FORMATOUTPUTSTREAM_H
#include "FormatBaseStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Encoder.h"

//#include "AV.h"
struct AVFormatContext;
struct AVOutputFormat;
namespace org{
    namespace esb{
	namespace av{
	    class FormatOutputStream: public FormatBaseStream, public org::esb::io::OutputStream{
		public:
		    FormatOutputStream(org::esb::io::File * target);
		    ~FormatOutputStream();
		    void write(char * buffer, int length);
		    void write(vector<unsigned char>&buffer);
		    void write(char * buffer);
		    void open();
		    void close();
		    void addPacketStream(PacketOutputStream & stream, Encoder & encoder);
//		private:
		    friend class PacketOutputStream;
		    AVFormatContext * _fmtCtx;
		    AVOutputFormat * _fmt;
			org::esb::io::File * _file;
	    };
	}
    }
};
#endif
