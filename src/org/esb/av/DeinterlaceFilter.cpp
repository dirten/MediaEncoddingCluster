#include "DeinterlaceFilter.h"

namespace org{
  namespace esb{
    namespace av{
      DeinterlaceFilter::DeinterlaceFilter(Format in, Format out):_in_format(in),_out_format(out){
      
      }
      void DeinterlaceFilter::close(){
      
      }
      std::string DeinterlaceFilter::getName(){
        return "DeinterlaceFilter";
      }
      void DeinterlaceFilter::open(){

      }
      void DeinterlaceFilter::reset(){

      }
      DeinterlaceFilter::ProcessStatus DeinterlaceFilter::process(Frame & in, Frame & out){
        out=in;
        if (avpicture_deinterlace((AVPicture*) in.getAVFrame(), (const AVPicture*) out.getAVFrame(), _in_format.pixel_format, _in_format.width, _in_format.height) < 0) {
          return BUFFER_PROCESSED_FAILED;
        }
        return BUFFER_PROCESSED_OK;
      }
    }
  }
}

