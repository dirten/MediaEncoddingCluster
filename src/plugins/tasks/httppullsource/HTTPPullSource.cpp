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
  }

  void HTTPPullSource::prepare() {
    Task::prepare();

    std::string data = getContext()->getEnvironment<std::string > ("data");
    if (libjson::is_valid(data)) {
      JSONNode node = libjson::parse(data);
      if (node.contains("url")) {
        _srcuristr = node["url"].as_string();
      }
    }
    if (_srcuristr.length() == 0) {
      throw org::esb::core::TaskException("No Source Url given!");
    }
  }

  int HTTPPullSource::getPadTypes() {
    return Task::SINK;
  }

  void HTTPPullSource::execute() {
    Task::execute();
    /*@TODO: implement the datasource*/
    org::esb::av::FormatInputStream fis(_srcuristr);

    if(!fis.isValid()){
      throw org::esb::core::TaskException("Input URI targets not a valid Media File!");
    }

    /*preprocessing the input streams*/
    int scount = fis.getStreamCount();
    std::map<int, Ptr<org::esb::av::Decoder> >decs;
    for (int a = 0; a < scount; a++) {
      /*getting the input stream from the file*/
      org::esb::av::AVInputStream* is = fis.getAVStream(a);
      decs[is->stream_identifier]=new org::esb::av::Decoder(is);
    }

    org::esb::av::PacketInputStream pis(&fis);
    org::esb::av::Packet * packet;
    while((packet = pis.readPacket()) != NULL){
      Ptr<org::esb::av::Packet> pPacket(packet);
      pPacket->_decoder=decs[pPacket->getStreamIndex()];
      pushBuffer(pPacket);
    }
  }

  REGISTER_TASK("HTTPPullSource", HTTPPullSource);
}

