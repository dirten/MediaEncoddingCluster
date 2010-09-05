#include "ResizeFilter.h"

namespace org{
  namespace esb{
    namespace av{
      ResizeFilter::ResizeFilter(Format in, Format out):_in_format(in),_out_format(out){
        _swsContext = NULL;
      }
      void ResizeFilter::close(){
        if (_swsContext)
          sws_freeContext(_swsContext);
        _swsContext = 0;
      }
      std::string ResizeFilter::getName(){
        return "ResizeFilter";
      }
      void ResizeFilter::open(){
        int sws_flags = 1;
        _swsContext = sws_getContext(
          _in_format.width, _in_format.height, _in_format.pixel_format, 
          _out_format.width, _out_format.height, _out_format.pixel_format, 
          sws_flags, NULL, NULL, NULL);

        if (_swsContext == NULL)
            LOGERROR("Could not initialize SWSCALE");        

      }
      void ResizeFilter::reset(){
      
      }
      ResizeFilter::ProcessStatus ResizeFilter::process(Frame & in_frame, Frame & out_frame){
        out_frame._type = in_frame._type;
        if (!_swsContext)return BUFFER_PROCESSED_FAILED;
        sws_scale(_swsContext, in_frame.getAVFrame()->data, in_frame.getAVFrame()->linesize, 0, in_frame.getHeight(), out_frame.getAVFrame()->data, out_frame.getAVFrame()->linesize);
        out_frame.setTimeBase(in_frame.getTimeBase());
        out_frame.pos = in_frame.pos;
        out_frame.setPts(in_frame.getPts());
        out_frame.setDts(in_frame.getDts());
        out_frame.stream_index = in_frame.stream_index;
        out_frame.duration = in_frame.duration;
        return BUFFER_PROCESSED_OK;
      }
      
    }
  }
}

