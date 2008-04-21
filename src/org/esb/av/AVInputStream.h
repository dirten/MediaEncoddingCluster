#ifndef ORG_ESB_AV_AVINPUTSTREAM_H
#define ORG_ESB_AV_AVINPUTSTREAM_H
#include "AV.h"
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

