#ifndef AVPIPE_H
#define AVPIPE_H
#include "org/esb/lang/Ptr.h"
#include <list>
namespace org {
  namespace esb {
    namespace av {
      class Packet;
      class Frame;
/**
  * @TODO : remove the shared Ptr behavior
  *
*/
      class AVPipe
      {
        public:
          AVPipe();
          //virtual ~AVPipe();
          void addTarget(AVPipe * pipe);

        protected:
          void pushPacket(Ptr<Packet> p);
          void pushFrame(Ptr<Frame> p);

        private:
          virtual bool newPacket(Ptr<Packet> p);
          virtual bool newFrame(Ptr<Frame> p);
          std::list< AVPipe * > _targets;
      };
    }
  }
}
#endif // AVPIPE_H
