/* 
 * File:   Pufs.cpp
 * Author: HoelscJ
 * 
 * Created on 22. September 2011, 10:36
 */

#include "org/esb/db/hivedb.hpp"
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
#include "org/esb/libjson/libjson.h"

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
  if (sreq->getRequestURI().find("/api/v1/pufs/pus") == 0) {
    std::string encid = sreq->getParameter("encid");
    JSONNode n(JSON_NODE);
    db::HiveDb db("sqlite3", org::esb::config::Config::get("db.url"));
    litesql::DataSource<db::Job>s = litesql::select<db::Job > (db, db::Job::Uuid == encid);
    if (s.count() == 1) {
      db::Job job = s.one();
      std::vector<db::Stream> streams = job.inputfile().get().one().streams().get().all();

      foreach(std::vector<db::Stream>::value_type & stream, streams) {
        if (stream.streamtype.value() == AVMEDIA_TYPE_VIDEO) {
          int sid = stream.id;
          LOGDEBUG("Video stream found " << sid);
          std::vector<db::ProcessUnit> units = litesql::select<db::ProcessUnit > (db, db::ProcessUnit::Sorcestream == sid).all();
          JSONNode n2(JSON_ARRAY);

          foreach(std::vector<db::ProcessUnit>::value_type & unit, units) {
            n2.push_back(JSONNode("", unit.id.value()));
          }
          n.push_back(n2);
        }
      }
    }
    org::esb::api::ServiceResponse*sres = ((org::esb::api::ServiceResponse*) res);
    n.push_back(JSONNode("requestId", sreq->getUUID()));
    sres->setStatus(org::esb::api::ServiceResponse::OK);
    sres->getOutputStream()->write(n.write_formatted());
  } else
    if (sreq->getRequestURI().find("/api/v1/pufs") == 0) {
    LOGDEBUG("PUFS called");
    org::esb::api::ServiceResponse*sres = ((org::esb::api::ServiceResponse*) res);
    sres->setStatus(org::esb::api::ServiceResponse::OK);
    int puid = atoi(sreq->getParameter("puid").c_str());
    int fnb = atoi(sreq->getParameter("frame").c_str());
    std::string type = sreq->getParameter("type");
    std::string name = org::esb::config::Config::getProperty("hive.tmp_path");
    name += "/";
    name += org::esb::util::StringUtil::toString(puid % 10);
    name += "/";
    name += org::esb::util::StringUtil::toString(puid);
    name += ".unit";
    org::esb::io::File f(name.c_str());
    if(!f.exists()){
      LOGERROR("file "<<f.getPath()<<" does not exist");
      return;
    }
    org::esb::io::FileInputStream fis(&f);
    org::esb::io::ObjectInputStream ois(&fis);
    org::esb::hive::job::ProcessUnit unit;

    ois.readObject(unit);
    //unit.process();
    unit._decoder->open();
    unit._encoder->open();
    if (unit._decoder->getCodecType() != AVMEDIA_TYPE_VIDEO) {
      LOGDEBUG("ProcessUnit " << f.getPath() << " is not a video");
      return;
    }
    boost::shared_ptr<Decoder >_refdecoder;
    if (unit._encoder->getCodecType() == AVMEDIA_TYPE_VIDEO) {
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
    enc.setTimeBase(1, 1);
    enc.setSink(&s);
    enc.setWidth(unit._decoder->getWidth());
    enc.setHeight(unit._decoder->getHeight());
    enc.open();

    org::esb::av::Encoder enc2(CODEC_ID_MJPEG);
    enc2.setPixelFormat(PIX_FMT_YUVJ420P);
    enc2.setBitRate(1000000);
    enc2.setTimeBase(1, 1);
    enc2.setSink(&s2);
    enc2.setWidth(unit._encoder->getWidth());
    enc2.setHeight(unit._encoder->getHeight());
    enc2.open();

    LOGDEBUG("ProcessUnit" << unit._decoder->toString());
    int c = 0;
    FrameConverter conv(unit._decoder.get(), unit._encoder.get());

    foreach(std::list<boost::shared_ptr<Packet> >::value_type & packet, unit._input_packets) {
      Frame * tmp = unit._decoder->decode2(*packet);
      if (tmp->isFinished()) {
        
        Frame * f = new Frame(unit._encoder->getInputFormat().pixel_format, unit._encoder->getWidth(), unit._encoder->getHeight());
        conv.convert(*tmp, *f);
        unit._encoder->encode(*f);
        //unit._encoder->encode();
         
        if (++c > fnb) {
          
          bool have_more_frames = true;
          while (have_more_frames) {
            if (unit._encoder->encode() <= 0) {
              have_more_frames = 0;
            }
          }

          Frame * tmp2 = NULL;

          foreach(const std::list<boost::shared_ptr<Packet> >::value_type & epacket, sout.getList()) {
            tmp2 = _refdecoder->decode2(*epacket);
          }
          bool last_packets = true;
          while (last_packets) {
            boost::shared_ptr<Packet >p = boost::shared_ptr<Packet > (new Packet());
            p->setTimeBase(unit._input_packets.front()->getTimeBase());
            p->setDuration(unit._input_packets.front()->getDuration());
            p->setStreamIndex(unit._input_packets.front()->getStreamIndex());
            Frame * lastf = _refdecoder->decode2(*p);
            if (!lastf->isFinished()) {
              last_packets = false;
            } else {
              tmp2 = lastf;
            }
          }
          sres->setMimetype("image/jpg");
          //LOGDEBUG(tmp->toString());
          std::string data;
          if (type == "dec") {
            enc.encode(*tmp);
            data = std::string((char*) s.getList().front()->getAVPacket()->data, s.getList().front()->getAVPacket()->size);
          } else {
            if (tmp2)
              enc2.encode(*tmp2);
            data = std::string((char*) s2.getList().front()->getAVPacket()->data, s2.getList().front()->getAVPacket()->size);
          }
          //LOGDEBUG(s2.getList().size());
          sres->getOutputStream()->write(data);
          //sres->getOutputStream()->write("test");
          break;
        }
      }
    }

  }
}


