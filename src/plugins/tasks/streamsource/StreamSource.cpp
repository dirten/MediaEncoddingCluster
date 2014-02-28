#include "StreamSource.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/core/TaskException.h"
#include "org/esb/av/PacketInputStream.h"
namespace plugin {

  StreamSource::StreamSource():Task()
  {
  }

  StreamSource::~StreamSource()
  {
  }

  void StreamSource::setSource(Ptr<FormatInputStream> fis){
    _fis=fis;
  }

  void StreamSource::prepare(){
    Task::prepare();
    _fis->dumpFormat();

    if(!_fis->isValid()){
      throw org::esb::core::TaskException("Input URI targets not a valid Media File!");
    }

    /*preprocessing the input streams*/
    int scount = _fis->getStreamCount();
    for (int a = 0; a < scount; a++) {
      /*getting the input stream from the file to extract the decoders*/
      org::esb::av::AVInputStream* is = _fis->getAVStream(a);
      if(is->codec->codec_type != AVMEDIA_TYPE_AUDIO && is->codec->codec_type != AVMEDIA_TYPE_VIDEO){
        continue;
      }
      _decs[is->index]=new org::esb::av::Decoder(is);
      _decs[is->index]->setStreamIndex(a);
      _decs[is->index]->open();
    }
    getContext()->setProperty<std::map<int, Ptr<org::esb::av::Decoder> > >("decoder",_decs);
  }

  void StreamSource::execute(){
    Task::execute();


    org::esb::av::PacketInputStream pis(_fis.get(), true);
    org::esb::av::Packet * packet;
    while((packet = pis.readPacket()) != NULL){
      Ptr<org::esb::av::Packet> pPacket(packet);
      if(_decs.size()>packet->getStreamIndex()){
        Ptr<org::esb::av::Decoder>dec=_decs[packet->getStreamIndex()];
        if(dec->getCodecType() != AVMEDIA_TYPE_AUDIO && dec->getCodecType() != AVMEDIA_TYPE_VIDEO){
          continue;
        }
        Task::pushBuffer(pPacket);
      }
    }
    Task::pushBuffer(Ptr<org::esb::av::Packet>(NULL));

  }

  int StreamSource::getPadTypes(){
    return Task::SINK;
  }

  org::esb::core::OptionsDescription StreamSource::getOptionsDescription(){
    org::esb::core::OptionsDescription result("httppullsource");
    return result;
  }
  REGISTER_TASK("StreamSource", StreamSource)

  }
