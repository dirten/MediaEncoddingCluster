
#include "BFrameProcessUnitFilter.h"
#include "Frame.h"
namespace org {
  namespace esb {
    namespace av {

      BFrameProcessUnitFilter::BFrameProcessUnitFilter(): AVFilter(VIDEO,""){

      }

      BFrameProcessUnitFilter::~BFrameProcessUnitFilter(){

      }

      bool BFrameProcessUnitFilter::newFrame(Ptr<Frame> p){

        if(!p){

          if(!previousFrame || previousFrame->getAVFrame()->pict_type==AV_PICTURE_TYPE_I){
            return pushFrame(Ptr<Frame>());
          }
        }
        if(previousFrame){
          Ptr<Frame> tmpFrame=previousFrame;
          previousFrame=p;
          return pushFrame(tmpFrame);
        }else{
          previousFrame=p;
        }
      }
    }
  }
}


