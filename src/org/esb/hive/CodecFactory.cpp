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
    sql::PreparedStatement stmt = con.prepareStatement("select codec, width, height, pix_fmt, bit_rate, time_base_num, time_base_den, gop_size, channels, sample_rate, sample_fmt from streams  where id=:id");
    stmt.setInt("id", streamid);
    sql::ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      boost::shared_ptr<av::Decoder> decoder(new av::Decoder((CodecID) rs.getInt(0)));
      decoder->setWidth(rs.getInt(1));
      decoder->setHeight(rs.getInt(2));
      decoder->setPixelFormat((PixelFormat) rs.getInt(3));
      decoder->setBitRate(rs.getInt(4));
      AVRational r;
      r.num = rs.getInt(5);
      r.den = rs.getInt(6);

      decoder->setTimeBase(r);
      decoder->setGopSize(rs.getInt(7));
      decoder->setChannels(rs.getInt(8));
      decoder->setSampleRate(rs.getInt(9));
      decoder->setSampleFormat((SampleFormat) rs.getInt(10));
      //    		decoder->open();
      decoder_map[streamid] = decoder;
    } else {
      throw std::runtime_error(string("no Decoder found for stream id "));
    }
  }
  return decoder_map[streamid].get();
}

av::Encoder * CodecFactory::getStreamEncoder(int streamid) {
  if (encoder_map.find(streamid) == encoder_map.end()) {
    sql::Connection con(config::Config::getProperty("db.connection"));
    sql::PreparedStatement stmt = con.prepareStatement("select codec, width, height, pix_fmt, bit_rate, time_base_num, time_base_den, gop_size, channels, sample_rate, sample_fmt from streams  where id=:id");
    stmt.setInt("id", streamid);
    sql::ResultSet rs = stmt.executeQuery();
    if (rs.next()) {
      boost::shared_ptr<av::Encoder> _encoder(new av::Encoder((CodecID) rs.getInt(0)));
      _encoder->setWidth(rs.getInt(1));
      _encoder->setHeight(rs.getInt(2));
      _encoder->setPixelFormat((PixelFormat) rs.getInt(3));
      _encoder->setBitRate(rs.getInt(4));
      AVRational r;
      r.num = rs.getInt(5);
      r.den = rs.getInt(6);

      _encoder->setTimeBase(r);
      _encoder->setGopSize(rs.getInt(7));
      _encoder->setChannels(rs.getInt(8));
      _encoder->setSampleRate(rs.getInt(9));
      _encoder->setSampleFormat((SampleFormat) rs.getInt(10));
      //    		_encoder->open();
      encoder_map[streamid] = _encoder;
    } else {
      throw std::runtime_error(string("no Encoder found for stream id "));
    }
  }
  return encoder_map[streamid].get();
}

