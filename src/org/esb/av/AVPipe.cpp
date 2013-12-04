#include "AVPipe.h"
#include "Packet.h"

namespace org {
  namespace esb {
    namespace av {

      AVPipe::AVPipe()
      {

      }

      bool AVPipe::newPacket(Ptr<Packet> p){
        return false;
      }

      bool AVPipe::newFrame(Ptr<Frame> p){
        return false;
      }

      void AVPipe::addTarget(AVPipe * pipe){
        _targets.push_back(pipe);
      }

      void AVPipe::pushPacket(Ptr<Packet> p){
        std::list<AVPipe *>::iterator it=_targets.begin();
        for(;it!=_targets.end();it++){
          (*it)->newPacket(p);
        }
      }

      void AVPipe::pushFrame(Ptr<Frame> p){
        std::list<AVPipe* >::iterator it=_targets.begin();
        for(;it!=_targets.end();it++){
          (*it)->newFrame(p);
        }
      }
    }
  }
}
