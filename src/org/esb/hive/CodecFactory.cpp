#include "CodecFactory.h"
#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include <stdexcept>


using namespace org::esb;
using namespace org::esb::hive;

std::map<int, boost::shared_ptr<org::esb::av::Decoder> > CodecFactory::decoder_map;
std::map<int, boost::shared_ptr<org::esb::av::Encoder> > CodecFactory::encoder_map;

av::Decoder * CodecFactory::getStreamDecoder(int streamid) {
  if (decoder_map.find(streamid) == decoder_map.end()) {
    sql::Connection con(config::Config::getProperty("db.connection"));
    sql::PreparedStatement stmt = con.prepareStatement("select codec, width, height, pix_fmt, bit_rate, time_base_num, time_base_den, gop_size, channels, sample_rate, sample_fmt, flags from streams  where id=:id");
    stmt.setInt("id", streamid);
    sql::ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      boost::shared_ptr<av::Decoder> decoder(new av::Decoder((CodecID) rs.getInt("codec")));
      decoder->setWidth(rs.getInt("width"));
      decoder->setHeight(rs.getInt("height"));
      decoder->setPixelFormat((PixelFormat) rs.getInt("pix_fmt"));
      decoder->setBitRate(rs.getInt("bit_rate"));
      AVRational r;
      r.num = rs.getInt("time_base_num");
      r.den = rs.getInt("time_base_den");

      decoder->setTimeBase(r);
      decoder->setGopSize(rs.getInt("gop_size"));
      decoder->setChannels(rs.getInt("channels"));
      decoder->setSampleRate(rs.getInt("sample_rate"));
      decoder->setSampleFormat((SampleFormat) rs.getInt("sample_fmt"));
      decoder->setFlag(rs.getInt("flags"));
      //    		decoder->open();
      decoder_map[streamid] = decoder;
    } else {
      logerror("no Decoder found for stream id "<<streamid);
      throw std::runtime_error(string("no Decoder found for stream id "));
    }
  }
  return decoder_map[streamid].get();
}

av::Encoder * CodecFactory::getStreamEncoder(int streamid) {
  if (encoder_map.find(streamid) == encoder_map.end()) {
    sql::Connection con(config::Config::getProperty("db.connection"));
    sql::PreparedStatement stmt = con.prepareStatement("select codec, width, height, pix_fmt, bit_rate, time_base_num, time_base_den, gop_size, channels, sample_rate, sample_fmt, flags from streams  where id=:id");
    stmt.setInt("id", streamid);
    sql::ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      boost::shared_ptr<av::Encoder> _encoder(new av::Encoder((CodecID) rs.getInt("codec")));
      _encoder->findCodec(org::esb::av::Codec::ENCODER);
      _encoder->setWidth(rs.getInt("width"));
      _encoder->setHeight(rs.getInt("height"));
      _encoder->setPixelFormat((PixelFormat) rs.getInt("pix_fmt"));
      _encoder->setBitRate(rs.getInt("bit_rate"));
      AVRational r;
      r.num = rs.getInt("time_base_num");
      r.den = rs.getInt("time_base_den");

      _encoder->setTimeBase(r);
      _encoder->setGopSize(rs.getInt("gop_size"));
      _encoder->setChannels(rs.getInt("channels"));
      _encoder->setSampleRate(rs.getInt("sample_rate"));
      _encoder->setSampleFormat((SampleFormat) rs.getInt("sample_fmt"));
      _encoder->setFlag(rs.getInt("flags"));
      //    		_encoder->open();
      encoder_map[streamid] = _encoder;
    } else {
      throw std::runtime_error(string("no Encoder found for stream id "));
    }
  }
  return encoder_map[streamid].get();
}
void CodecFactory::free(){
/*
  std::map<int, boost::shared_ptr<org::esb::av::Decoder> >::iterator it_dec=decoder_map.begin();
  for(;it_dec!=decoder_map.end();it_dec++){
    delete (*it_dec).second;
  }
  std::map<int, boost::shared_ptr<org::esb::av::Decoder> >::iterator it_enc=encoder_map.begin();
  for(;it_enc!=encoder_map.end();it_enc++){
    delete (*it_enc).second;
  }
   */
}
