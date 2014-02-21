#ifndef BFRAMEPROCESSUNITFILTER_H
#define BFRAMEPROCESSUNITFILTER_H


#include "../AVFilter.h"
#include "../exports.h"

namespace org {
  namespace esb {
    namespace av {

      class AV_EXPORT BFrameProcessUnitFilter : public AVFilter {
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
