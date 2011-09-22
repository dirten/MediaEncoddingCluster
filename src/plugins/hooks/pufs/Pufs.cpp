/* 
 * File:   Pufs.cpp
 * Author: HoelscJ
 * 
 * Created on 22. September 2011, 10:36
 */

#include "Pufs.h"
#include "plugins/services/webservice/ServiceRequest.h"
#include "plugins/services/webservice/ServiceResponse.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/config/config.h"
class PacketSink : public Sink {
public:

  PacketSink() {
  }

  void write(void * p) {
    Packet* pt = (Packet*) p;
    boost::shared_ptr<Packet> pEnc(new Packet(*pt));
    pkts.push_back(pEnc);
  }

  std::list<boost::shared_ptr<Packet> > getList() {
    return pkts;
  }
private:
  std::list<boost::shared_ptr<Packet> > pkts;

};

Pufs::Pufs() {
}

Pufs::~Pufs() {
}

void Pufs::handleRequest(org::esb::core::Request * req, org::esb::core::Response*res) {
  org::esb::api::ServiceRequest*sreq = ((org::esb::api::ServiceRequest*) req);
  if (sreq->getRequestURI().find("/api/v1/pufs") == 0) {
    LOGDEBUG("PUFS called");
    org::esb::api::ServiceResponse*sres = ((org::esb::api::ServiceResponse*) res);
    sres->setStatus(org::esb::api::ServiceResponse::OK);
    int puid = atoi(sreq->getParameter("puid").c_str());
    int fnb = atoi(sreq->getParameter("frame").c_str());
    std::string name = org::esb::config::Config::getProperty("hive.base_path");
    name += "/tmp/";
    name += org::esb::util::StringUtil::toString(puid % 10);
    name += "/";
    name += org::esb::util::StringUtil::toString(puid);
    name += ".unit";
    org::esb::io::File f(name.c_str());
    org::esb::io::FileInputStream fis(&f);
    org::esb::io::ObjectInputStream ois(&fis);
    org::esb::hive::job::ProcessUnit unit;
    
    ois.readObject(unit);
    unit.process();
    unit._decoder->open();
    unit._encoder->open();
    boost::shared_ptr<Decoder >_refdecoder;
    if ( unit._encoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
    std::map<std::string, std::string>opt = unit._encoder->getCodecOptions();
    _refdecoder = boost::shared_ptr<Decoder > (new Decoder(unit._encoder->getCodecId()));
    std::map<std::string, std::string>::iterator opit = opt.begin();
    _refdecoder->setWidth(unit._encoder->getWidth());
    _refdecoder->setHeight(unit._encoder->getHeight());
    _refdecoder->setPixelFormat(unit._encoder->getPixelFormat());

    for (; opit != opt.end(); opit++) {
      if ((*opit).first != "extradata" || (*opit).first != "extradata_size")
        _refdecoder->setCodecOption((*opit).first, (*opit).second);
    }
    LOGDEBUG("EncoderExtrdataSize:" << unit._encoder->ctx->extradata_size);
    LOGDEBUG("RefDecoderExtrdataSize:" << _refdecoder->ctx->extradata_size);
    //std::cout << _encoder->ctx->extradata;
    _refdecoder->ctx->extradata = static_cast<uint8_t*> (av_malloc(unit._encoder->ctx->extradata_size));
    memcpy(_refdecoder->ctx->extradata, unit._encoder->ctx->extradata, unit._encoder->ctx->extradata_size);
    //_refdecoder->ctx->extradata[0] = 2;
    //_refdecoder->ctx->extradata_size=0;
    _refdecoder->open();
  }

    PacketSink s;
    PacketSink sout;
    PacketSink s2;
    unit._encoder->setSink(&sout);
    org::esb::av::Encoder enc(CODEC_ID_MJPEG);
    enc.setPixelFormat(PIX_FMT_YUVJ420P);
    enc.setBitRate(1000000);
    enc.setTimeBase(1,1);
    enc.setSink(&s);
    enc.setWidth(unit._decoder->getWidth());
    enc.setHeight(unit._decoder->getHeight());
    enc.open();

    org::esb::av::Encoder enc2(CODEC_ID_MJPEG);
    enc2.setPixelFormat(PIX_FMT_YUVJ420P);
    enc2.setBitRate(1000000);
    enc2.setTimeBase(1,1);
    enc2.setSink(&s2);
    enc2.setWidth(unit._encoder->getWidth());
    enc2.setHeight(unit._encoder->getHeight());
    enc2.open();
    
    LOGDEBUG("ProcessUnit"<<unit._decoder->toString());
    int c=0;
    foreach(std::list<boost::shared_ptr<Packet> >::value_type & packet, unit._input_packets){
        Frame * tmp = unit._decoder->decode2(*packet);
        Frame * tmp2=NULL;
        if(tmp->isFinished()){
          unit._encoder->encode(*tmp);
          //if(sout.getList().size()>0)
          //      tmp2=_refdecoder->decode2(*sout.getList().back().get());
        }
        if(tmp->isFinished()&&++c>fnb){
          sres->setMimetype("image/bmp");
          LOGDEBUG(tmp->toString());
          enc.encode(*tmp);
          //if(tmp2)
          //      enc2.encode(*tmp2);
          LOGDEBUG(s.getList().size());
          std::string data((char*)s.getList().front()->getAVPacket()->data, s.getList().front()->getAVPacket()->size);
          sres->getOutputStream()->write(data);
          //sres->getOutputStream()->write("test");
          break;
        }
    }
    
  }
}


