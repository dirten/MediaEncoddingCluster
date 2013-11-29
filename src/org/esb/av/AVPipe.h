#ifndef AVPIPE_H
#define AVPIPE_H
#include "org/esb/lang/Ptr.h"
#include <list>
namespace org {
  namespace esb {
    namespace av {
      class Packet;
      class Frame;

      class AVPipe
      {
        public:
          AVPipe();
          void addTarget(Ptr<AVPipe> pipe);
        protected:
          void pushPacket(Ptr<Packet> p);
          virtual void newPacket(Ptr<Packet> p);
          void pushFrame(Ptr<Frame> p);
          virtual void newFrame(Ptr<Frame> p);

        private:
          std::list< Ptr<AVPipe> > _targets;
      };
    }
  }
}
#endif // AVPIPE_H
