#ifndef ORG_ESB_AV_FRAMECONVERTER_H
#define ORG_ESB_AV_FRAMECONVERTER_H

extern "C" {
#include "swscale.h"
//#include "avformat.h"
}


//struct SwsContext;

namespace org{
namespace esb{
namespace av{

    class Frame;
    class FrameFormat;

    class FrameConverter{
	public:
		FrameConverter(FrameFormat& targetFormat);
		Frame convert(Frame & input);
	private:
	    void getSwsContext();
	    SwsContext * _swsContext;
	    FrameFormat * _inFormat;
	    FrameFormat * _outFormat;
    };
}}}


#endif


