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

//std::map<int, boost::shared_ptr<org::esb::av::Decoder> > CodecFactory::decoder_map;
//std::map<int, boost::shared_ptr<org::esb::av::Encoder> > CodecFactory::encoder_map;

boost::shared_ptr<org::esb::av::Decoder> CodecFactory::getStreamDecoder(int streamid) {
  //if (decoder_map.find(streamid) == decoder_map.end()) {
  boost::shared_ptr<av::Decoder> decoder;
  try {
    db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
    db::Stream stream = litesql::select<db::Stream > (db, db::Stream::Id == streamid).one();

    decoder = boost::shared_ptr<av::Decoder > (new av::Decoder((CodecID) (int) stream.codecid));
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

    //db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
    //db::Stream stream = litesql::select<db::Stream > (db, db::Stream::Id == streamid).one();

    vector<db::StreamParameter> params = stream.params().get().all();
    vector<db::StreamParameter>::iterator it = params.begin();
    std::multimap<std::string, std::string> pmap;
    for (; it != params.end(); it++) {
      pmap.insert(std::pair<std::string, std::string > ((*it).name.value(), (*it).val.value()));
    }
    setCodecOptions(decoder, pmap);

    decoder->ctx->extradata_size = stream.extradatasize;
    if (decoder->ctx->extradata_size > 0) {
      decoder->ctx->extradata = (uint8_t*) av_malloc(decoder->ctx->extradata_size);
      memcpy(decoder->ctx->extradata, ((std::string)stream.extradata).c_str(), decoder->ctx->extradata_size);
    } else
      decoder->ctx->extradata = NULL;


  } catch (litesql::NotFound e) {
    LOGERROR("no Decoder found for stream id " << streamid);
  }
  //}
  return decoder;
}

boost::shared_ptr<org::esb::av::Encoder> CodecFactory::getStreamEncoder(std::multimap<std::string, std::string> pmap) {
  boost::shared_ptr<av::Encoder> encoder(new av::Encoder((CodecID) atoi((*pmap.find("codec_id")).second.c_str())));
  encoder->findCodec(org::esb::av::Codec::ENCODER);
  if (encoder->getCodecType() == CODEC_TYPE_VIDEO) {
    if (pmap.count("width"))
      encoder->setWidth(atoi((*pmap.find("width")).second.c_str()));
    if (pmap.count("height"))
      encoder->setHeight(atoi((*pmap.find("height")).second.c_str()));
    encoder->setPixelFormat((PixelFormat) 0);
    if (encoder->_codec->pix_fmts) {
      encoder->setPixelFormat(encoder->_codec->pix_fmts[0]);
    }
  }
  if (encoder->getCodecType() == CODEC_TYPE_AUDIO) {
    if (encoder->_codec->sample_fmts) {
      encoder->setSampleFormat(encoder->_codec->sample_fmts[0]);
    }
  }
  setCodecOptions(encoder, pmap);
  return encoder;
}

boost::shared_ptr<org::esb::av::Encoder> CodecFactory::getStreamEncoder(int streamid) {
  LOGTRACEMETHOD("CodecFactory::getStreamEncoder(int streamid)")
  std::multimap<std::string, std::string> pmap;
  try {
    db::HiveDb db = org::esb::hive::DatabaseService::getDatabase();
    db::Stream stream = litesql::select<db::Stream > (db, db::Stream::Id == streamid).one();

    vector<db::StreamParameter> params = stream.params().get().all();
    vector<db::StreamParameter>::iterator it = params.begin();
    for (; it != params.end(); it++) {
      pmap.insert(std::pair<std::string, std::string > ((*it).name.value(), (*it).val.value()));
      LOGDEBUG("Loaded Codec Param key="<<(*it).name.value()<<" val="<<(*it).val.value());
    }
  } catch (litesql::NotFound e) {
    LOGERROR("no Encoder found for stream id " << streamid);
  }
  return getStreamEncoder(pmap);
}

void CodecFactory::setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>enc, std::vector<db::StreamParameter> p) {
  CodecPropertyTransformer transformer(p);
  std::map<std::string, std::string> params = transformer.getCodecProperties();
  std::map<std::string, std::string>::iterator it = params.begin();
  for (; it != params.end(); it++) {
    if ((*it).second.length() > 0) {
      if (enc->setCodecOption((*it).first, (*it).second)) {
        LOGERROR("setting CodecOptionsPair (opt=" << (*it).first << " arg=" << (*it).second << ")");
      }
    }
  }
}

void CodecFactory::setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>enc, std::multimap<std::string, std::string> param) {
  CodecPropertyTransformer transformer(param);
  std::map<std::string, std::string> params = transformer.getCodecProperties();
  std::map<std::string, std::string>::iterator it = params.begin();
  for (; it != params.end(); it++) {
    if ((*it).second.length() > 0) {
      if (enc->setCodecOption((*it).first, (*it).second)) {
        LOGERROR("setting CodecOptionsPair (opt=" << (*it).first << " arg=" << (*it).second << ")");
      }
    }
  }
}

void CodecFactory::setCodecOptions(boost::shared_ptr<org::esb::av::Decoder>_dec, std::multimap<std::string, std::string> param) {
  CodecPropertyTransformer transformer(param);
  std::map<std::string, std::string> params = transformer.getCodecProperties();
  std::map<std::string, std::string>::iterator it = params.begin();
  for (; it != params.end(); it++) {
    if ((*it).second.length() > 0) {
      if (_dec->setCodecOption((*it).first, (*it).second)) {
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
  /*
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
   */
}

void CodecFactory::clearCodec(int streamid) {
  /*
  if (decoder_map.find(streamid) != decoder_map.end()) {
    decoder_map[streamid].reset();
    decoder_map.erase(streamid);
  }
  if (encoder_map.find(streamid) != encoder_map.end()) {
    encoder_map[streamid].reset();
    encoder_map.erase(streamid);
    //    delete (encoder_map.find(streamid)->second()->get());
  }*/
}
