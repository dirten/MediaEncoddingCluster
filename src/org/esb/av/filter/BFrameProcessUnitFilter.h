#ifndef BFRAMEPROCESSUNITFILTER_H
#define BFRAMEPROCESSUNITFILTER_H


#include "../AVFilter.h"

namespace org {
  namespace esb {
    namespace av {

      class BFrameProcessUnitFilter : public AVFilter {
        public:
          BFrameProcessUnitFilter();
          ~BFrameProcessUnitFilter();
          bool newFrame(Ptr<Frame> p);
          void init(){/*NOP*/}
        private:
          Ptr<Frame> previousFrame;
      };
    }
  }
}
#endif // BFRAMEPROCESSUNITFILTER_H
