#include "AVPipe.h"
#include "Packet.h"

namespace org {
  namespace esb {
    namespace av {

      AVPipe::AVPipe()
      {

      }

      void AVPipe::newPacket(Packet * p){

      }

      void AVPipe::newFrame(Frame * p){

      }
      void AVPipe::newPacket(AVPacket * p){

      }

      void AVPipe::newFrame(AVFrame * p){

      }

      void AVPipe::addTarget(AVPipe * pipe){
        _targets.push_back(pipe);
      }

      void AVPipe::pushPacket(Packet * p){
        std::list<AVPipe *>::iterator it=_targets.begin();
        for(;it!=_targets.end();it++){
          (*it)->newPacket(p);
        }
      }

      void AVPipe::pushFrame(Frame * p){
        std::list<AVPipe* >::iterator it=_targets.begin();
        for(;it!=_targets.end();it++){
          (*it)->newFrame(p);
        }
      }
      void AVPipe::pushPacket(AVPacket * p){
        std::list<AVPipe *>::iterator it=_targets.begin();
        for(;it!=_targets.end();it++){
          (*it)->newPacket(p);
        }
      }

      void AVPipe::pushFrame(AVFrame * p){
        std::list<AVPipe* >::iterator it=_targets.begin();
        for(;it!=_targets.end();it++){
          (*it)->newFrame(p);
        }
      }
    }
  }
}
