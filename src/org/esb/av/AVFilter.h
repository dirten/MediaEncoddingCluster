#ifndef AVFILTER_H
#define AVFILTER_H

#include "AVPipe.h"
namespace org {
  namespace esb {
    namespace av {
      class AVFilter: public AVPipe
      {
        public:
          AVFilter();
          void newFrame(Ptr<Frame> p);
      };

    }
  }
}
#endif // AVFILTER_H
