
#include "BFrameProcessUnitFilter.h"
namespace org {
  namespace esb {
    namespace av {

      BFrameProcessUnitFilter::BFrameProcessUnitFilter(): AVFilter(VIDEO,""){

      }

      BFrameProcessUnitFilter::~BFrameProcessUnitFilter(){

      }

      bool BFrameProcessUnitFilter::newFrame(Ptr<Frame> p){
        return pushFrame(p);
      }
    }
  }
}


