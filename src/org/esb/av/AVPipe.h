#ifndef AVPIPE_H
#define AVPIPE_H
#include "org/esb/lang/Ptr.h"
#include "AV.h"
#include "Depricated.h"
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
          DEPRICATED(void pushPacket(Packet * p));
          DEPRICATED(virtual void newPacket(Packet * p));
          DEPRICATED(void pushFrame(Frame * p));
          DEPRICATED(virtual void newFrame(Frame * p));

          void pushPacket(AVPacket * p);
          virtual void newPacket(AVPacket * p);
          void pushFrame(AVFrame * p);
          virtual void newFrame(AVFrame * p);
        private:
          std::list< AVPipe * > _targets;
      };
    }
  }
}
#endif // AVPIPE_H
