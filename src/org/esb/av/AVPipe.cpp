#include "AVPipe.h"
#include "Packet.h"

namespace org {
  namespace esb {
    namespace av {

      AVPipe::AVPipe()
      {

      }

      void AVPipe::newPacket(Ptr<Packet> p){

      }

      void AVPipe::newFrame(Ptr<Frame> p){

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
