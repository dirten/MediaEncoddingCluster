#include "org/esb/db/hivedb.hpp"
#include "CodecFactory.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/config/config.h"
#include "org/esb/util/StringTokenizer.h"
#include "DatabaseService.h"
#include <stdexcept>


using namespace org::esb;
using namespace org::esb::hive;

std::map<int, boost::shared_ptr<org::esb::av::Decoder> > CodecFactory::decoder_map;
std::map<int, boost::shared_ptr<org::esb::av::Encoder> > CodecFactory::encoder_map;

boost::shared_ptr<org::esb::av::Decoder> CodecFactory::getStreamDecoder(int streamid) {
  if (decoder_map.find(streamid) == decoder_map.end()) {
    try {
      db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
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

boost::shared_ptr<org::esb::av::Encoder> CodecFactory::getStreamEncoder(int streamid) {
  if (encoder_map.find(streamid) == encoder_map.end()) {
    try {
      db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
      db::Stream stream = litesql::select<db::Stream > (db, db::Stream::Id == streamid).one();

      boost::shared_ptr<av::Encoder> _encoder(new av::Encoder((CodecID) (int)stream.codecid));
      _encoder->findCodec(org::esb::av::Codec::ENCODER);
      _encoder->setWidth(stream.width);
      _encoder->setHeight(stream.height);
      _encoder->setPixelFormat((PixelFormat) (int)stream.pixfmt);
      _encoder->setBitRate(stream.bitrate);

      _encoder->setTimeBase(stream.framerateden, stream.frameratenum);
      if (stream.streamtype == CODEC_TYPE_VIDEO) {
        _encoder->setFrameRate(stream.frameratenum, stream.framerateden);
      } else {
        _encoder->setTimeBase(stream.streamtimebasenum, stream.streamtimebaseden);
      }

      _encoder->setGopSize(stream.gopsize);
      _encoder->setChannels(stream.channels);
      _encoder->setSampleRate(stream.samplerate);
      _encoder->setSampleFormat((SampleFormat) (int)stream.samplefmt);
      _encoder->setFlag(stream.flags);

      setCodecOptions(_encoder, stream.extraprofileflags);
      //    		_encoder->open();
      encoder_map[streamid] = _encoder;
    } catch (litesql::NotFound e) {
      LOGERROR("no Encoder found for stream id " << streamid);
      //      throw std::runtime_error(string("no Encoder found for stream id "));
    }
  }
  return encoder_map[streamid];
}

void CodecFactory::setCodecOptions(boost::shared_ptr<org::esb::av::Encoder>_enc, std::string options) {
  if (options.length() > 0) {
    org::esb::util::StringTokenizer to(options, ";");
    while (to.hasMoreTokens()) {
      std::string line = to.nextToken();
      if(line.length()==0)continue;
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
  
    std::map<int, boost::shared_ptr<org::esb::av::Decoder> >::iterator it_dec=decoder_map.begin();
    for(;it_dec!=decoder_map.end();it_dec++){
      (*it_dec).second.reset();
    }
    std::map<int, boost::shared_ptr<org::esb::av::Encoder> >::iterator it_enc=encoder_map.begin();
    for(;it_enc!=encoder_map.end();it_enc++){
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
