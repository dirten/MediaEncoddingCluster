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
        int a=1;
        while ((_last_packet = _pis->readPacket()) != NULL ) {
          if(_last_packet->getStreamIndex()==_stream_info->getIndex()){
            _frame=_decoder->decode2(*_last_packet.get());
            if(_frame->isFinished()&& a>=width){
              break;
            }
            a++;
          }
        }
        if(_frame&&!_frame->isFinished()){
          _frame=new org::esb::av::Frame(_decoder->getOutputFormat().pixel_format,_decoder->getWidth(), _decoder->getHeight());
        }
      }

      /**
       *
       * @TODO: tracking a list of keyframes is needed for the seeking back to the previous keyframe in MPEG2 Streams.
       * AVSEEK_FLAG_BACKWARD seems not to be seeking correctly
       */
      void PreviewFrameServer::jumpPrevious(JUMP_WIDTH width){
        AVRational frame_rate;
        frame_rate.num=_stream_info->getFrameRate().den;
        frame_rate.den=_stream_info->getFrameRate().num;
        AVRational time_base=_stream_info->getTimeBase();
        int duration=av_rescale_q(1,frame_rate, time_base);
        LOGDEBUG("Seek duration:"<<duration<<" seek width:"<<width);
        int64_t seek_ts=_last_packet->getDts()-(duration*width);
        LOGDEBUG("seek stream "<<_stream_info->getIndex()<<" to ts "<<seek_ts);
        _fis->seek(_stream_info->getIndex(), seek_ts);



        int a=1;
        while ((_last_packet = _pis->readPacket()) != NULL ) {
          if(_last_packet->getStreamIndex()==_stream_info->getIndex()){
            LOGDEBUG(_last_packet->toString());
            _frame=_decoder->decode2(*_last_packet.get());
            if(_frame->isFinished()&& _last_packet->getDts()>=seek_ts){
              break;
            }
            a++;
          }
        }
        if(_frame&&!_frame->isFinished()){
          _frame=Ptr<org::esb::av::Frame>(new org::esb::av::Frame());
        }
      }
    }
  }
}


