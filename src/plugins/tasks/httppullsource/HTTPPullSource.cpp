/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "HTTPPullSource.h"
#include "org/esb/core/TaskException.h"
//#include "org/esb/lang/Ptr.h"

#include "org/esb/libjson/libjson.h"



//#include "org/esb/io/File.h"
//#include "org/esb/util/StringUtil.h"
//#include "org/esb/config/config.h"
//#include "Poco/URI.h"

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
namespace plugin {

  HTTPPullSource::HTTPPullSource() : Task() {
  }

  HTTPPullSource::~HTTPPullSource() {
    LOGDEBUG("  HTTPPullSource::~HTTPPullSource()")
    std::map<int, Ptr<org::esb::av::Decoder> >::iterator deit=_decs.begin();
    for(;deit!=_decs.end();deit++){
        (*deit).second->close();
      }
    //delete _fis;
  }

  void HTTPPullSource::prepare() {
    Task::prepare();

    std::string data = getContext()->getEnvironment<std::string > ("data");
    if (libjson::is_valid(data)) {
      JSONNode node = libjson::parse(data);
      if (node.contains("srcurl")) {
        _srcuristr = node["srcurl"].as_string();
      }
    }else{
      throw org::esb::core::TaskException("no valid json !");
    }
    if (_srcuristr.length() == 0) {
      throw org::esb::core::TaskException("No Source Url given!");
    }
    /*openning the input stream*/
    _fis=new org::esb::av::FormatInputStream(_srcuristr);
    if(!_fis->isValid()){
      throw org::esb::core::TaskException("Input URI targets not a valid Media File!");
    }

    /*preprocessing the input streams*/
    int scount = _fis->getStreamCount();
    for (int a = 0; a < scount; a++) {
      /*getting the input stream from the file*/
      org::esb::av::AVInputStream* is = _fis->getAVStream(a);
      _decs[is->index]=new org::esb::av::Decoder(is);
      _decs[is->index]->setStreamIndex(a);
      _decs[is->index]->open();

      //setBufferCodec(is->stream_identifier,_decs[is->stream_identifier]);
    }
    getContext()->setProperty<std::map<int, Ptr<org::esb::av::Decoder> > >("decoder",_decs);
    /*std::map<int, Ptr<org::esb::av::Decoder> >tmp=getContext()->get<std::map<int, Ptr<org::esb::av::Decoder> > >("decoder");
    std::map<int, Ptr<org::esb::av::Decoder> >::iterator it1=_decs.begin();
    for(;it1!=_decs.end();it1++){
      LOGDEBUG("Decoder:"<<(*it1).first<<":"<<(*it1).second->toString());
    }
    std::map<int, Ptr<org::esb::av::Decoder> >::iterator it=tmp.begin();
    for(;it!=tmp.end();it++){
      LOGDEBUG("Decoder:"<<(*it).first<<":"<<(*it).second->toString());
    }*/
  }

  org::esb::core::OptionsDescription HTTPPullSource::getOptionsDescription() {
    org::esb::core::OptionsDescription result("httppullsource");
    result.add_options()
    ("data", boost::program_options::value<std::string > ()->default_value(""), "HTTPPullSource task data");
    return result;
  }

  int HTTPPullSource::getPadTypes() {
    return Task::SINK;
  }

  void HTTPPullSource::execute() {
    Task::execute();


    org::esb::av::PacketInputStream pis(_fis.get());
    org::esb::av::Packet * packet;
    while((packet = pis.readPacket()) != NULL){
      //pPacket->_decoder=_decs[pPacket->getStreamIndex()];
      //LOGDEBUG("Push buffer")
      Ptr<org::esb::av::Decoder>dec=_decs[packet->getStreamIndex()];
      if(dec->getCodecType() != AVMEDIA_TYPE_AUDIO && dec->getCodecType() != AVMEDIA_TYPE_VIDEO){
        continue;
      }
      Ptr<org::esb::av::Packet> pPacket(packet);
      Task::pushBuffer(pPacket);
    }
    Task::pushBuffer(Ptr<org::esb::av::Packet>(NULL));
  }

  REGISTER_TASK("HTTPPullSource", HTTPPullSource)
}

