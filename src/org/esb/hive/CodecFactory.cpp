#include "CodecFactory.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringTokenizer.h"
#include "DatabaseService.h"
#include <stdexcept>
#include "CodecPropertyTransformer.h"

using namespace org::esb;
using namespace org::esb::hive;

std::map<int, boost::shared_ptr<org::esb::av::Decoder> > CodecFactory::decoder_map;
std::map<int, boost::shared_ptr<org::esb::av::Encoder> > CodecFactory::encoder_map;

boost::shared_ptr<org::esb::av::Decoder> CodecFactory::getStreamDecoder(int streamid) {
  if (decoder_map.find(streamid) == decoder_map.end()) {
    try {
      db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
      db::Stream stream = litesql::select<db::Stream > (db, db::Stream::Id == streamid).one();

      boost::shared_ptr<av::Decoder> decoder(new av::Decoder((CodecID) (int) stream.codecid));
      decoder->setWidth(stream.width);
      decoder->setHeight(stream.height);
      decoder->setPixelFormat((PixelFormat) (int) stream.pixfmt);
      decoder->setBitRate(stream.bitrate);
      if (stream.streamtype == CODEC_TYPE_VIDEO) {
        decoder->setTimeBase(stream.codectimebasenum, stream.codectimebaseden);
        decoder->setFrameRate(stream.frameratenum, stream.framerateden);
      } else {
        decoder->setTimeBase(stream.streamtimebasenum, stream.streamtimebaseden);
      }
      decoder->setGopSize(stream.gopsize);
      decoder->setChannels(stream.channels);
      decoder->setSampleRate(stream.samplerate);
      decoder->setSampleFormat((SampleFormat) (int) stream.samplefmt);
      decoder->setFlag(stream.flags);
      decoder->setBitsPerCodedSample(stream.bitspercodedsample);
      decoder->ctx->ticks_per_frame = stream.ticksperframe;
      decoder->ctx->extradata_size = stream.extradatasize;
      if (decoder->ctx->extradata_size > 0) {
        decoder->ctx->extradata = (uint8_t*) av_malloc(decoder->ctx->extradata_size);
        memcpy(decoder->ctx->extradata, (char*) ((std::string)stream.extradata).c_str(), decoder->ctx->extradata_size);
      } else
        decoder->ctx->extradata = NULL;
      decoder_map[streamid] = decoder;
    } catch (litesql::NotFound e) {
      LOGERROR("no Decoder found for stream id " << streamid);
    }
  }
  return decoder_map[streamid];
}

boost::shared_ptr<org::esb::av::Encoder> CodecFactory::getStreamEncoder(std::multimap<std::string, std::string> pmap) {
  boost::shared_ptr<av::Encoder> _encoder(new av::Encoder((CodecID) atoi((*pmap.find("codec_id")).second.c_str())));
  _encoder->findCodec(org::esb::av::Codec::ENCODER);
  if(_encoder->getCodecType()==CODEC_TYPE_VIDEO){
    if(pmap.count("width"))
      _encoder->setWidth(atoi((*pmap.find("width")).second.c_str()));
    if(pmap.count("height"))
      _encoder->setHeight(atoi((*pmap.find("height")).second.c_str()));
    _encoder->setPixelFormat((PixelFormat) 0);
    if (_encoder->_codec->pix_fmts) {
      _encoder->setPixelFormat(_encoder->_codec->pix_fmts[0]);
    }
  }
  if(_encoder->getCodecType()==CODEC_TYPE_AUDIO){
    if (_encoder->_codec->sample_fmts) {
      _encoder->setSampleFormat(_encoder->_codec->sample_fmts[0]);
    }
  }
  setCodecOptions(_encoder, pmap);
  return _encoder;
}

boost::shared_ptr<org::esb::av::Encoder> CodecFactory::getStreamEncoder(int streamid) {
  LOGTRACEMETHOD("CodecFactory::getStreamEncoder(int streamid)")
  if (encoder_map.find(streamid) == encoder_map.end()) {
    try {
      db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
      db::Stream stream = litesql::select<db::Stream > (db, db::Stream::Id == streamid).one();

      vector<db::StreamParameter> params = stream.params().get().all();
      vector<db::StreamParameter>::iterator it = params.begin();
      std::multimap<std::string, std::string> pmap;
      for (; it != params.end(); it++) {
        pmap.insert(std::pair<std::string, std::string>((*it).name.value(),(*it).val.value()));
      }
      boost::shared_ptr<av::Encoder> _encoder = getStreamEncoder(pmap);
      /*
      boost::shared_ptr<av::Encoder> _encoder(new av::Encoder((CodecID) atoi(pmap["codec_id"].c_str())));
      _encoder->findCodec(org::esb::av::Codec::ENCODER);
      _encoder->setWidth(atoi(pmap["width"].c_str()));
      _encoder->setHeight(atoi(pmap["height"].c_str()));
      _encoder->setPixelFormat((PixelFormat) 0);
      if(_encoder->_codec->pix_fmts){
        _encoder->setPixelFormat(_encoder->_codec->pix_fmts[0]);
      }
      if(_encoder->_codec->sample_fmts){
        _encoder->setSampleFormat(_encoder->_codec->sample_fmts[0]);
      }*/
      //      _encoder->setBitRate(stream.bitrate);
      /*
      _encoder->setTimeBase(stream.framerateden, stream.frameratenum);
      if (stream.streamtype == CODEC_TYPE_VIDEO) {
        _encoder->setFrameRate(stream.frameratenum, stream.framerateden);
      } else {
        _encoder->setTimeBase(stream.streamtimebasenum, stream.streamtimebaseden);
      }*/

      //      _encoder->setGopSize(stream.gopsize);
      //      _encoder->setChannels(stream.channels);
      //      _encoder->setSampleRate(stream.samplerate);
      //_encoder->setSampleFormat((SampleFormat) 1);
      //      _encoder->setFlag(stream.flags);
      //      vector<db::StreamParameter> params = stream.params().get().all();
//      setCodecOptions(_encoder, stream.params().get().all());



      //      setCodecOptions(_encoder, stream.params().get().all());
      //    		_encoder->open();
      encoder_map[streamid] = _encoder;
    } catch (litesql::NotFound e) {
      LOGERROR("no Encoder found for stream id " << streamid);
      //      throw std::runtime_error(string("no Encoder found for stream id "));
    }
  }
  return encoder_map[streamid];
}

void CodecFactory::setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>_enc, std::vector<db::StreamParameter> p) {
  CodecPropertyTransformer transformer(p);
  std::map<std::string, std::string> params = transformer.getCodecProperties();
  std::map<std::string, std::string>::iterator it = params.begin();
  for (; it != params.end(); it++) {
    if ((*it).second.length() > 0) {
      if (_enc->setCodecOption((*it).first, (*it).second)) {
        LOGERROR("setting CodecOptionsPair (opt=" << (*it).first << " arg=" << (*it).second << ")");
      }
    }
  }
}
void CodecFactory::setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>_enc,std::multimap<std::string, std::string> param) {
  CodecPropertyTransformer transformer(param);
  std::map<std::string, std::string> params = transformer.getCodecProperties();
  std::map<std::string, std::string>::iterator it = params.begin();
  for (; it != params.end(); it++) {
    if ((*it).second.length() > 0) {
      if (_enc->setCodecOption((*it).first, (*it).second)) {
        LOGERROR("setting CodecOptionsPair (opt=" << (*it).first << " arg=" << (*it).second << ")");
      }
    }
  }
}

void CodecFactory::setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>_enc, std::string options) {
  if (options.length() > 0) {
    org::esb::util::StringTokenizer to(options, ";");
    while (to.hasMoreTokens()) {
      std::string line = to.nextToken();
      if (line.length() == 0)continue;
      org::esb::util::StringTokenizer to2(line, "=");
      if (to2.countTokens() != 2) {
        LOGWARN("Invalid CodecOptionsPair it is not a <key=value> pair ---" << line);
      } else {
        std::string opt = to2.nextToken();
        std::string arg = to2.nextToken();
        if (_enc->setCodecOption(opt, arg)) {
          LOGERROR("setting CodecOptionsPair (opt=" << opt << " arg=" << arg << ")");
        }
      }
    }
  }
}

void CodecFactory::free() {

  std::map<int, boost::shared_ptr<org::esb::av::Decoder> >::iterator it_dec = decoder_map.begin();
  for (; it_dec != decoder_map.end(); it_dec++) {
    (*it_dec).second.reset();
  }
  std::map<int, boost::shared_ptr<org::esb::av::Encoder> >::iterator it_enc = encoder_map.begin();
  for (; it_enc != encoder_map.end(); it_enc++) {
    (*it_enc).second.reset();
  }
  decoder_map.clear();
  encoder_map.clear();

}

void CodecFactory::clearCodec(int streamid) {
  if (decoder_map.find(streamid) != decoder_map.end()) {
    decoder_map[streamid].reset();
    decoder_map.erase(streamid);
  }
  if (encoder_map.find(streamid) != encoder_map.end()) {
    encoder_map[streamid].reset();
    encoder_map.erase(streamid);
    //    delete (encoder_map.find(streamid)->second()->get());
  }
}
