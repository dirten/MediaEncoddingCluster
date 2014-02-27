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
          void init(){/*NOP*/}
        private:
          bool newFrame(Ptr<Frame> p);
          Ptr<Frame> previousFrame;
      };
    }
  }
}
#endif // BFRAMEPROCESSUNITFILTER_H
