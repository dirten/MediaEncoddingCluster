#include "FrameConverter.h"
#include "FrameFormat.h"
#include "Frame.h"
#include "swscale.h"

namespace org{
namespace esb{
namespace av{

    FrameConverter::FrameConverter(FrameFormat & out_format){
	_swsContext=0;
    }

    Frame FrameConverter::convert(Frame & in_frame){
    
    }

    void FrameConverter::getSwsContext(){
	int sws_flags=0;

	_swsContext = sws_getCachedContext(_swsContext,
            _inFormat->width, _inFormat->height,
            _inFormat->pixel_format,
            _outFormat->width, _outFormat->height,
            _outFormat->pixel_format, sws_flags, NULL, NULL, NULL);
//	return _swsContext;
    }

}}}

