#include "AVPipe.h"
#include "Packet.h"

namespace org {
  namespace esb {
    namespace av {

      AVPipe::AVPipe()
      {

      }
      AVPipe::~AVPipe()
      {
        clearTargets();
      }

      void AVPipe::clearTargets(){
        _targets.clear();
      }

      /*
      bool AVPipe::newPacket(Ptr<Packet> p){
        return false;
      }

      bool AVPipe::newFrame(Ptr<Frame> p){
        return false;
      }*/

      void AVPipe::addTarget(AVPipe * pipe){
        _targets.push_back(pipe);
      }

      bool AVPipe::pushPacket(Ptr<Packet> p){
        bool result=false;
        std::list<AVPipe *>::iterator it=_targets.begin();
        for(;it!=_targets.end();it++){
          result|=(*it)->newPacket(p);
        }
        return result;
      }

      bool AVPipe::pushFrame(Ptr<Frame> p){
        bool result=false;
        std::list<AVPipe* >::iterator it=_targets.begin();
        for(;it!=_targets.end();it++){
          result|=(*it)->newFrame(p);
        }
        return result;
      }
    }
  }
}
