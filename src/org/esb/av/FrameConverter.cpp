#include "FrameConverter.h"
#include "FrameFormat.h"
#include "Frame.h"
//#include "swscale.h"
#include <iostream>
namespace org{
namespace esb{
namespace av{

    FrameConverter::FrameConverter(FrameFormat & out_format){
		_swsContext=0;
		_outFormat=&out_format;
		
    }
    FrameConverter::~FrameConverter(){
		if(_swsContext);
			av_free(_swsContext);
		_swsContext=0;
		
    }

    Frame FrameConverter::convert(Frame & in_frame){
		/*
		if(in_frame._type!=CODEC_TYPE_VIDEO){
			std::cerr <<"Kein Video"<<std::endl;
			return in_frame;
		}*/
//		std::cerr <<"Video"<<std::endl;
		Frame out_frame(_outFormat->pixel_format,_outFormat->width, _outFormat->height);
/*
		ImgReSampleContext * resampling_ctx = 
			img_resample_init(_outFormat->width, _outFormat->height, in_frame.getWidth(), in_frame.getHeight());
*/
		int sws_flags=1;
		_swsContext = sws_getCachedContext(_swsContext,
            in_frame.getWidth(), in_frame.getHeight(),
            (PixelFormat)in_frame.getFormat(),
            _outFormat->width, _outFormat->height,
            _outFormat->pixel_format, sws_flags, NULL, NULL, NULL);

//		getSwsContext();
    	sws_scale(_swsContext,in_frame.data, in_frame.linesize,0,in_frame.getHeight(),out_frame.data, out_frame.linesize);
    	out_frame.pos=in_frame.pos;
    	out_frame.pts=in_frame.pts;
    	out_frame.dts=in_frame.dts;
		return out_frame;
    }

    void FrameConverter::getSwsContext(){
//	return _swsContext;
    }

}}}

