#ifndef ORG_ESB_AV_FRAMECONVERTER_H
#define ORG_ESB_AV_FRAMECONVERTER_H

#include "AV.h"
#include "Encoder.h"
#include "Decoder.h"

//struct SwsContext;

namespace org{
namespace esb{
namespace av{

    class Frame;
    struct FrameFormat;

    class FrameConverter{
	public:
		FrameConverter(FrameFormat& targetFormat);
//		FrameConverter(FrameFormat& sourceFormat,FrameFormat& targetFormat);
		FrameConverter(Decoder* dec,Encoder* enc);
		~FrameConverter();
		Frame convert(Frame & input);
	private:
		Frame convertVideo(Frame & input);
		Frame convertAudio(Frame & input);
//	    void getSwsContext();
	    SwsContext * _swsContext;
	    FrameFormat * _inFormat;
	    FrameFormat * _outFormat;
            Decoder * _dec;
            Encoder * _enc;

	    ReSampleContext * _audioCtx;
    };
}}}


#endif


