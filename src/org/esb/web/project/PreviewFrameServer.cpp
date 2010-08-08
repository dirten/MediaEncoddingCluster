#include "PreviewFrameServer.h"
#include "org/esb/av/StreamInfo.h"

namespace org{
  namespace esb{
    namespace web{
      using namespace org::esb::av;
      PreviewFrameServer::PreviewFrameServer(){

      }
      Ptr<org::esb::av::Frame> PreviewFrameServer::getFrame(){
        return _frame;
      }

      Ptr<org::esb::av::Decoder> PreviewFrameServer::getDecoder(){
        return _decoder;
      }
      bool PreviewFrameServer::isOpen(){
        return(_fis.get()!=NULL && _fis->isValid());
      }
      void PreviewFrameServer::setFileName(std::string filename){
        File infile(filename);
        _fis=Ptr<FormatInputStream>(new FormatInputStream(&infile));
        _pis=Ptr<PacketInputStream>(new PacketInputStream(_fis.get()));

        int c=_fis->getStreamCount();
        for(int a = 0;a<c;a++){
          _stream_info=Ptr<org::esb::av::StreamInfo>(_fis->getStreamInfo(a));
          if(_stream_info->getCodecType()==CODEC_TYPE_VIDEO){
            _decoder=Ptr<Decoder>(new Decoder(_fis->getAVStream(_stream_info->getIndex())));
            _decoder->open();
            LOGDEBUG("Video Stream found!");
            break;
          }
        }
        if(isOpen())
          jumpForward(JUMP_NONE);
      }
      void PreviewFrameServer::jumpForward(JUMP_WIDTH width){
        Packet * packet;
        Frame * frame=NULL;
        int a=0;
        while ((packet = _pis->readPacket()) != NULL ) {
          if(packet->getStreamIndex()==_stream_info->getIndex()){
            frame=_decoder->decode2(*packet);
            if(frame->isFinished()&& width<=a){
              _last_packet=Ptr<org::esb::av::Packet>(packet);
              break;
            }
            delete frame;
            frame=NULL;
            a++;
          }
        }
        if(frame&&!frame->isFinished()){
          delete frame;
          _frame=Ptr<org::esb::av::Frame>(new org::esb::av::Frame());
        }else{
          _frame=Ptr<org::esb::av::Frame>(frame);
        }
      }
      void PreviewFrameServer::jumpPrevious(JUMP_WIDTH width){

      }
    }
  }
}


