/* 
 * File:   TestFullTranscoding.cpp
 * Author: jhoelscher
 *
 * Created on 23. Oktober 2009, 10:17
 */

#include <stdlib.h>
#include <map>

#include "org/esb/util/Log.h"

#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/FormatOutputStream.h"

#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/PacketOutputStream.h"

#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"

#include "org/esb/av/FrameConverter.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/AVFilter.h"

#include "org/esb/util/StringUtil.h"
#include "org/esb/hive/Environment.h"
//#include <thread>
using namespace org::esb::av;
//using org::esb::av::AVFilter;
using org::esb::util::StringUtil;
using org::esb::hive::Environment;
struct StreamData {
  Decoder * dec;
  Encoder * enc;
  FrameConverter * conv;
  org::esb::av::AVFilter * filter;
  int64_t start_dts;
  bool more_frames;
};

map<int, StreamData> _sdata;
map<int, int> _smap;

/*
 *
 */
int main(int argc, char** argv) {
  Environment::build(argc,argv);
  Log::open("");

  /*open the fixed test File or the file from command line input*/
  std::string src;
  std::string trg;
  if (argc == 1) {
    src = MEC_SOURCE_DIR;
    src.append("/test-data/test.dvd");
    trg = MEC_SOURCE_DIR;
    trg.append("/test2.mp4");
  } else {
    src = argv[1];
    trg = argv[2];
  }


  /*opening the input file and Packet Input Stream*/
  File f(src.c_str());
  FormatInputStream fis(&f);
  PacketInputStream pis(&fis, true, true);


  /*opening the output file and Packet Output Stream*/
  File f_out(trg.c_str());
  FormatOutputStream fos(&f_out);
  PacketOutputStream pos(&fos);
  //std::thread t;
  /*Create and open the input and output Codecs*/
  int c = fis.getStreamCount();
  int s = 0;
  bool video = false, audio = false;
  for (int i = 0; i < c; i++) {
    if (fis.getStreamInfo(i)->getCodecType() != AVMEDIA_TYPE_VIDEO &&
        fis.getStreamInfo(i)->getCodecType() != AVMEDIA_TYPE_AUDIO) continue;
    //if(i!=4&&i!=1)continue;
    if (audio && video)continue;
    fis.dumpFormat();
    _sdata[i].filter=NULL;
    _sdata[i].dec = new Decoder(fis.getAVStream(i));
    _sdata[i].dec->setStreamIndex(i);
    _sdata[i].start_dts = fis.getStreamInfo(i)->getFirstDts();
    _sdata[i].enc = new Encoder();
    _sdata[i].more_frames = true;
    if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_VIDEO) {
      _sdata[i].enc->setStreamIndex(s);
      video = true;
      _sdata[i].enc->setCodecId(CODEC_ID_MPEG4);
      //_sdata[i].enc->setCodecId(CODEC_ID_THEORA);
      _sdata[i].enc->setWidth(720);
      _sdata[i].enc->setHeight(576);
      //      _sdata[i].enc->setWidth(320);
      //      _sdata[i].enc->setHeight(240);
      _sdata[i].enc->setGopSize(200);
      _sdata[i].enc->setCodecOption("flags","+psnr");
      AVRational ar;
      ar.num = 1;
      ar.den = 25;
      //ar.den = _sdata[i].dec->getTimeBase().den/_sdata[i].dec->ctx->ticks_per_frame;

      _sdata[i].enc->setTimeBase(ar);
      _sdata[i].enc->setBitRate(1500000);



      _sdata[i].filter=new org::esb::av::AVFilter(VIDEO,"scale=%width%:%height%");

      //char buf[512];
      //av_get_channel_layout_string(buf, sizeof(buf), _sdata[i].enc->getChannels(), _sdata[i].enc->getChannelLayout());
      _sdata[i].filter->setInputParameter("width",StringUtil::toString(_sdata[i].dec->getWidth()));
      _sdata[i].filter->setInputParameter("height",StringUtil::toString(_sdata[i].dec->getHeight()));
      _sdata[i].filter->setInputParameter("pixel_format",StringUtil::toString(_sdata[i].dec->getPixelFormat()));
      _sdata[i].filter->setInputParameter("time_base", StringUtil::toString(_sdata[i].dec->getTimeBase().num)+"/"+StringUtil::toString(_sdata[i].dec->getTimeBase().den));
      _sdata[i].filter->setInputParameter("sample_aspect_ratio", StringUtil::toString(_sdata[i].dec->ctx->sample_aspect_ratio.num)+"/"+StringUtil::toString(_sdata[i].dec->ctx->sample_aspect_ratio.den));

      _sdata[i].filter->setOutputParameter("width",StringUtil::toString(_sdata[i].enc->getWidth()));
      _sdata[i].filter->setOutputParameter("height",StringUtil::toString(_sdata[i].enc->getHeight()));
      _sdata[i].filter->setOutputParameter("pixel_format",StringUtil::toString(_sdata[i].enc->getPixelFormat()));




      //_sdata[i].filter=NULL;

      // logdebug(_sdata[i].enc->toString());
    } else if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_AUDIO) {
      audio = true;
      _sdata[i].enc->setStreamIndex(s);

      _sdata[i].enc->setCodecId(CODEC_ID_MP2);
      _sdata[i].enc->setBitRate(128000);
      _sdata[i].enc->setSampleRate(44100);
      _sdata[i].enc->setChannels(2);
      _sdata[i].enc->setChannelLayout(AV_CH_LAYOUT_STEREO);

      //_sdata[i].enc->setSampleFormat(_sdata[i].dec->getSampleFormat());


      _sdata[i].filter=new org::esb::av::AVFilter(AUDIO,"aresample=%sample_rate%,aformat=sample_fmts=%sample_format%:channel_layouts=%channel_layout%");

      char buf[512];
      av_get_channel_layout_string(buf, sizeof(buf), _sdata[i].enc->getChannels(), _sdata[i].dec->getChannelLayout());
      int64_t ch_layout=_sdata[i].dec->getChannelLayout();
      //_sdata[i].filter->setInputParameter("channel_layout",buf);
      _sdata[i].filter->setInputParameter("channel_layout",StringUtil::toString(ch_layout));
      _sdata[i].filter->setInputParameter("sample_rate",StringUtil::toString(_sdata[i].dec->getSampleRate()));
      _sdata[i].filter->setInputParameter("sample_format", av_get_sample_fmt_name(_sdata[i].dec->getSampleFormat()));
      _sdata[i].filter->setInputParameter("time_base", "1/"+StringUtil::toString(fis.getStreamInfo(i)->getTimeBase().den));

      _sdata[i].filter->setOutputParameter("channel_layout","stereo");
      _sdata[i].filter->setOutputParameter("channel_layout2","stereo");
      _sdata[i].filter->setOutputParameter("frame_size",StringUtil::toString(_sdata[i].enc->ctx->frame_size));
      _sdata[i].filter->setOutputParameter("sample_rate",StringUtil::toString(_sdata[i].enc->getSampleRate()));

      //_sdata[i].filter=NULL;
      //          _sdata[i].enc->open();
      AVRational ar;
      ar.num = 1;
      ar.den = _sdata[i].dec->getSampleRate();
      _sdata[i].dec->setTimeBase(ar);

    }
    if (fos._fmt->flags & AVFMT_GLOBALHEADER)
      _sdata[i].enc->setFlag(CODEC_FLAG_GLOBAL_HEADER);


    _sdata[i].dec->open();
    _sdata[i].enc->open();

    if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_AUDIO){
      _sdata[i].filter->setOutputParameter("sample_format", av_get_sample_fmt_name(_sdata[i].enc->getSampleFormat()));
      _sdata[i].filter->setOutputParameter("frame_size",StringUtil::toString(_sdata[i].enc->ctx->frame_size));
    }
    _smap[i] = s++;
    //if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_VIDEO)
    //if (_sdata[i].dec->getCodecType() == AVMEDIA_TYPE_VIDEO)
    _sdata[i].conv = new FrameConverter(_sdata[i].dec, _sdata[i].enc);


    /*buidl the processing chain*/
    /* decoder -> converter -> encoder -> outputstream */
    if(_sdata[i].filter){
      _sdata[i].filter->init();
      _sdata[i].dec->addTarget(_sdata[i].filter);
      _sdata[i].filter->addTarget(_sdata[i].enc);
    }else{
      _sdata[i].dec->addTarget(_sdata[i].conv);
      _sdata[i].conv->addTarget(_sdata[i].enc);
    }
    _sdata[i].enc->addTarget(&pos);

    pos.setEncoder(*_sdata[i].enc, _sdata[i].enc->getStreamIndex());
    //_sdata[i].enc->setOutputStream(&pos);
    LOGDEBUG(_sdata[i].enc->toString());
    LOGDEBUG(_sdata[i].dec->toString());
  }

  pos.init();
  fos.dumpFormat();

  /*build the processing chain*/

  /*main loop to encode the packets*/
  Packet *packet;
  bool encode_whole_file=false;
  for (int i = 0; i < 100 || encode_whole_file; i++) {
    //reading a packet from the Stream
    if ((packet=pis.readPacket()) ==NULL )break; //when no more packets available(EOF) then it return <0
    boost::shared_ptr<Packet> p(packet);
    int idx=p->getStreamIndex();
    if (_sdata.count(idx) == 0)continue;

    /*simply pushing the packet into the decoder to process the complete chain*/
    _sdata[idx].dec->newPacket(p);
    continue;


    //    p.setDts(p.getDts() - _sdata[p.getStreamIndex()].start_dts);
    //Decoding a Video or Audio Packet
    //LOGDEBUG("Packet:"<<p->toString());
    Frame * src_frame = _sdata[idx].dec->decode2(*p);

    if (!src_frame->isFinished()) {
      delete src_frame;

      continue;
    }
    //mapping input tp output stream
    src_frame->stream_index = _smap[idx];

    //Convert an Audio or Video Packet
    Frame * trg_frame = NULL;
    if (_sdata[idx].dec->getCodecType() == AVMEDIA_TYPE_VIDEO)
      trg_frame = new Frame(
            _sdata[idx].enc->getInputFormat().pixel_format,
            _sdata[idx].enc->getWidth(),
            _sdata[idx].enc->getHeight());
    if (_sdata[idx].dec->getCodecType() == AVMEDIA_TYPE_AUDIO)
      trg_frame = new Frame();

    if (_sdata[idx].conv) {
      _sdata[idx].conv->convert(*src_frame, *trg_frame);
    }else{
      *trg_frame=*src_frame;
    }
    int ret = _sdata[idx].enc->encode(*trg_frame);

    if(src_frame)
      delete src_frame;
    src_frame=NULL;
    if(trg_frame)
      delete trg_frame;
    trg_frame=NULL;
    //    delete p;

  }

  map<int, StreamData>::iterator it = _sdata.begin();
  for (; it != _sdata.end(); it++) {
    StreamData s = (*it).second;
    while(s.dec->newPacket(new Packet()));
  }
  //int data_size=0;

cleanup:
  pos.close();
  fos.close();

  map<int, StreamData>::iterator streams = _sdata.begin();
  for (; streams != _sdata.end(); streams++) {
    (*streams).second.enc->close();
    (*streams).second.dec->close();
    delete (*streams).second.conv;
    delete (*streams).second.filter;
    delete (*streams).second.enc;
    //delete (*streams).second.enc;
    delete (*streams).second.dec;


  }
  //  Log::close();
  return (EXIT_SUCCESS);
}

