#include "ProcessUnit.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameFormat.h"
#include "org/esb/av/FrameConverter.h"
#include "org/esb/av/Sink.h"

#include "org/esb/util/Log.h"
using namespace org::esb::hive::job;
using namespace org::esb::av;

bool toDebug = true;

class PacketSink : public Sink {
public:

  PacketSink() {
  }

  void write(void * p) {
    logdebug("Write Packet to Term Sink");
    Packet* pt = (Packet*) p;
    boost::shared_ptr<Packet> pEnc(new Packet(*pt));
    pkts.push_back(pEnc);
    if (toDebug) {
      logdebug("outputpacket");
      pEnc->toString();

    }

    //    erl_print_term((FILE*) stderr, pkts.back());
  }

  std::list<boost::shared_ptr<Packet> > getList() {
    return pkts;
  }
private:
  std::list<boost::shared_ptr<Packet> > pkts;

};

ProcessUnit::ProcessUnit() {
  _decoder = NULL;
  _encoder = NULL;
  codec = NULL;
  _target_stream = 0;
  _source_stream = 0;
  _frame_group = 0;
  _frame_count = 0;
  _process_unit = 0;

  id = 0;
}

ProcessUnit::~ProcessUnit() {
  /*
          if(_decoder!=NULL)
                  delete _decoder;
          if(_encoder!=NULL)
                  delete _encoder;
          if(codec!=NULL)
                  delete codec;
   */
  //    _decoder=NULL;
  //    _encoder=NULL;

  //    _target_stream=0;
  //    _input_packets.clear();
  //    _output_packets.clear();

}

void ProcessUnit::process() {
  int insize = 0, outsize = 0;
  av_register_all();
  avcodec_init();
  avcodec_register_all();

  if (_decoder != NULL)
    _decoder->open();
  if (_encoder != NULL)
    _encoder->open();
  if (toDebug)
    logdebug("Codex openned");
  _decoder->ctx->request_channel_layout = 2;

  PacketSink sink;
  _encoder->setSink(&sink);

  FrameConverter conv(_decoder, _encoder);
  if (toDebug)
    logdebug("Converter created");

  //	cout << "start decoding encoding"<<endl;
  list< boost::shared_ptr<Packet> >::iterator it;
  multiset<boost::shared_ptr<Frame>, PtsComparator > pts_list;
  multiset<boost::shared_ptr<Packet>, PtsPacketComparator > pts_packets;
  int a = 0;
  int counter = 0, s = 0;
  for (it = _input_packets.begin(); it != _input_packets.end(); it++) {
    if (toDebug)
      logdebug("Loop");
    boost::shared_ptr<Packet> p = *it;
    if (++s % 100 == 0)
      cout << "\r" << s;
    insize += p->packet->size;
    if (toDebug) {
      logdebug("inputpacket")
      p->toString();
    }

    if (p->isKeyFrame()) {
      //      cout << "KeyFrame\t";
    }
    //	    if(tmp._buffer==0)continue;
    //        boost::shared_ptr<Frame> fr(new Frame(tmp));
    //        pts_list.insert(fr);

    //    cout << "PacketPts:" << p->packet->pts << "\tPacketDts:" << p->packet->dts << "\t";
    //	    cout <<"\tFramePts:"<<tmp.getPts()<<"\tFrameDts:"<<tmp.getDts();
    //	    cout <<"\tFrame*Pts:"<<fr->getPts()<<"\tFrame*Dts:"<<fr->getDts();
    //        cout << endl;

    Frame * tmp = _decoder->decode2(*p);
    if (toDebug){
      logdebug("Frame Decoded");
      tmp->toString();
    }
    //    if (_frame_count >= counter && tmp.pict_type == FF_I_TYPE) {
    //      break;
    //    }
    if (!tmp->isFinished()) {
      continue;
    }
    if (toDebug)
      logdebug("Frame Buffer > 0");



    //	    fr->setPts(++a);
    //	    fr->setDts(AV_NOPTS_VALUE);
    Frame * f;
    if (_decoder->ctx->codec_type == CODEC_TYPE_VIDEO)
      f = new Frame(_encoder->getPixelFormat(), _encoder->getWidth(), _encoder->getHeight());
    if (_decoder->ctx->codec_type == CODEC_TYPE_AUDIO)
      f = new Frame();
    if (toDebug)
      logdebug("try Frame Convert");
    conv.convert(*tmp, *f);
    delete tmp;
    if (toDebug){
      logdebug("Frame Converted");
      f->toString();
    }

//    f->setPts(f->getDts());
    //	    f.setPts(++a);
    //	    tmp.setDts(AV_NOPTS_VALUE);
    Packet ret = _encoder->encode(*f);
    delete f;
    //cout << "PacketPts:" << ret.packet->pts << "\tPacketDts:" << ret.packet->dts << "\t";
    if (toDebug)
      logdebug("Frame Encoded");

    //    boost::shared_ptr<Packet> pEnc(new Packet(ret));
    if (toDebug)
      logdebug("Packet Created");

    //	    pEnc->packet->dts=AV_NOPTS_VALUE;
    outsize += ret.packet->size;
    //    _output_packets.push_back(pEnc);
    _output_packets = sink.getList();
    if (toDebug)
      logdebug("Packet Added");

    //	    cout <<"FramePts:"<<f.pts<<"\tFrameDts:"<<f.dts;
    //	    cout <<"\tPacketPts:"<<pEnc->packet->pts<<"\tPacketDts:"<<pEnc->packet->dts<<endl;


    //        pts_packets.insert(p);
  }

  //	cout <<"ListSize:"<<_input_packets.size()<<"\tSetSize:"<<pts_list.size()<<endl;

  //	multiset<boost::shared_ptr<Frame>, PtsComparator>::iterator pts_it;
  /*
          multiset<boost::shared_ptr<Packet>, PtsPacketComparator>::iterator packet_it;
          a=0;

          for(packet_it=pts_packets.begin();packet_it!=pts_packets.end();packet_it++){
              boost::shared_ptr<Packet> p=*packet_it;
              insize+=p->packet->size;
              Frame tmp=_decoder->decode(*p);
              if(tmp._buffer==0)continue;

          boost::shared_ptr<Frame> fr(new Frame(tmp));
  //	    fr->setDts(++a);

          pts_list.insert(fr);

      
              tmp.setPts(++a);
              tmp.setDts(AV_NOPTS_VALUE);
                  Frame f=conv.convert(tmp);
              Packet ret=_encoder->encode(f);
              boost::shared_ptr<Packet> pEnc(new Packet(ret));
              outsize+=pEnc->packet->size;
              _output_packets.push_back(pEnc);
      
      }
   */
  /*
          a=0;
          for(pts_it=pts_list.begin();pts_it!=pts_list.end();pts_it++){
              boost::shared_ptr<Frame> tmp=*pts_it;
              tmp->setPts(++a);
  //	    tmp->setDts(AV_NOPTS_VALUE);
	    
                  Frame f=conv.convert(*tmp);
  //		f.dts=f.pts;
              Packet ret=_encoder->encode(*tmp);
              boost::shared_ptr<Packet> pEnc(new Packet(ret));
              outsize+=pEnc->packet->size;
              pEnc->packet->dts=AV_NOPTS_VALUE;
              _output_packets.push_back(pEnc);
  //	    cout <<"FramePts:"<<f.pts<<"\tFrameDts:"<<f.dts;
              cout <<"\tPacketPts:"<<pEnc->packet->pts<<"\tPacketDts:"<<pEnc->packet->dts<<endl;
  //	    cout << "Frame*Pts:"<<tmp->pts<<"\tPacketPts:"<<ret.packet->pts<<"Packet*Pts"<<pEnc->packet->pts<<endl;
          }
   */
  logdebug("InputSize:" << insize << "OutputSize:" << outsize);

  if (_decoder != NULL)
    delete _decoder;
  if (_encoder != NULL)
    delete _encoder;
  _decoder = 0;
  _encoder = 0;


}
