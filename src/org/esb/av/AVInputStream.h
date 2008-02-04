#ifndef ORG_ESB_AV_AVINPUTSTREAM_H
#define ORG_ESB_AV_AVINPUTSTREAM_H
extern "C" {
#include "avformat.h"
#include "avcodec.h"
}
#include "Codec.h"
namespace org{
    namespace esb{
	namespace av{
	    class AVInputStream:public AVStream{
		public:
		    Codec * getCodec();
		    void setFormatContext(AVFormatContext * ctx);
		private:
		    AVFormatContext *_formatContext;
	    };
	}
    }
}


#endif

