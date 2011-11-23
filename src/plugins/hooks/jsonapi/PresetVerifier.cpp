/* 
 * File:   PresetVerifier.cpp
 * Author: HoelscJ
 * 
 * Created on 19. September 2011, 13:37
 */

#include "PresetVerifier.h"
#include "org/esb/hive/PresetReaderJson.h"
#include "org/esb/hive/CodecFactory.h"
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/av/AV.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/av/FormatOutputStream.h"
#include "org/esb/av/PacketOutputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"
namespace org {
  namespace esb {
    namespace plugin {

      std::string PresetVerifier::verify(JSONNode&root) {
        std::string result;
        org::esb::hive::PresetReaderJson reader(root.write());
        org::esb::hive::PresetReaderJson::CodecList codecs = reader.getCodecList();
        org::esb::hive::PresetReaderJson::FilterList filters = reader.getFilterList();
        org::esb::hive::PresetReaderJson::Preset pre = reader.getPreset();
        LOGDEBUG("searching for Format id:" << pre["id"]);
        /*resolving the outputformat to have knowledge of the global header flag*/
        AVOutputFormat *ofmt = NULL;
        while ((ofmt = av_oformat_next(ofmt))) {
          LOGDEBUG(ofmt->name);
          if (pre["id"] == ofmt->name) {
            break;
          }
        }
        if (!ofmt) {
          result = "Could not find Output Format";
          LOGERROR(result);
          return result;
        }
        if (ofmt->flags & AVFMT_GLOBALHEADER) {
          codecs["video"].insert(std::pair<std::string, std::string > ("global_header", "1"));
          codecs["audio"].insert(std::pair<std::string, std::string > ("global_header", "1"));
        }
        typedef std::multimap<std::string, std::string>::iterator rowtype;
        //std::multimap<std::string, std::string>::iterator sdata = codec.begin();
        /*search for width and height*/
        if (codecs["video"].count("width") == 0 ||
                atoi((*codecs["video"].find("width")).second.c_str()) == 0 ||
                codecs["video"].count("height") == 0 ||
                atoi((*codecs["video"].find("height")).second.c_str()) == 0) {
          if (codecs["video"].count("width") > 0)
            codecs["video"].erase(codecs["video"].find("width"));
          if (codecs["video"].count("height") > 0)
            codecs["video"].erase(codecs["video"].find("height"));
          codecs["video"].insert(std::pair<std::string, std::string > ("width", "320"));
          codecs["video"].insert(std::pair<std::string, std::string > ("height", "240"));
        }
        if (codecs["video"].count("time_base") == 0 || (*codecs["video"].find("time_base")).second.length() == 0) {
          if (codecs["video"].count("time_base") > 0)
            codecs["video"].erase(codecs["video"].find("time_base"));


          codecs["video"].insert(std::pair<std::string, std::string > ("time_base", "1/25"));
        }
        /*
        AVCodec * vcodec = avcodec_find_encoder_by_name((*codecs["video"].find("codec_id")).second.c_str());
        if (vcodec) {
          codecs["video"].erase(codecs["video"].find("codec_id"));
          codecs["video"].insert(std::pair<std::string, std::string > ("codec_id", org::esb::util::StringUtil::toString(vcodec->id)));

        }
        AVCodec * acodec = avcodec_find_encoder_by_name((*codecs["audio"].find("codec_id")).second.c_str());
        if (acodec) {
          codecs["audio"].erase(codecs["audio"].find("codec_id"));
          codecs["audio"].insert(std::pair<std::string, std::string > ("codec_id", org::esb::util::StringUtil::toString(acodec->id)));

        }
         */
        boost::shared_ptr<org::esb::av::Encoder> venc = boost::shared_ptr<org::esb::av::Encoder>(new org::esb::av::Encoder(codecs["video"]["codec_id"]));//org::esb::hive::CodecFactory::getStreamEncoder(codecs["video"]);
        boost::shared_ptr<org::esb::av::Encoder> aenc = boost::shared_ptr<org::esb::av::Encoder>(new org::esb::av::Encoder(codecs["audio"]["codec_id"]));//org::esb::hive::CodecFactory::getStreamEncoder(codecs["audio"]);

        foreach(rowtype::value_type & row, codecs["video"]) {
          venc->setCodecOption(row.first, row.second);
        }

        foreach(rowtype::value_type & row, codecs["audio"]) {
          aenc->setCodecOption(row.first, row.second);
        }
        if (!venc->open()) {
          result = "Could not open video encoder, please check your profile for the video encoder";
          LOGERROR(result);
          return result;
        }
        if (!aenc->open()) {
          result = "Could not open audio encoder, please check your profile for the audio encoder";
          LOGERROR(result);
          return result;
        }
        /*openning the OutputStreams*/
        org::esb::io::File fout(org::esb::config::Config::get("hive.tmp_path")+"/test");

        org::esb::av::FormatOutputStream * fos = new org::esb::av::FormatOutputStream(&fout, ofmt->name);
        org::esb::av::PacketOutputStream * pos = new org::esb::av::PacketOutputStream(fos);
        pos->setEncoder(*venc.get(),0);
        pos->setEncoder(*aenc.get(),1);
        if(!pos->init()){
          result = "Could not initialize outputfile, please check your profile for more information look into the server logfile";
          LOGERROR(result);
          return result;          
        }
        org::esb::av::Packet vp;
        vp.setStreamIndex(0);
        vp.setTimeBase(1,25);
        org::esb::av::Packet ap;
        ap.setStreamIndex(1);
        ap.setTimeBase(1,25);
        if(pos->writePacket(vp)){
          result = "unable to write a video packet to the outputstream, please check your profile for more information look into the server logfile";
          LOGERROR(result);
          return result;                    
        }
        if(pos->writePacket(ap)){
          result = "unable to write an audio packet to the outputstream, please check your profile for more information look into the server logfile";
          LOGERROR(result);
          return result;          
        }
        if(!pos->close()){
          result = "unable to write the trailer to the outputstream, please check your profile for more information look into the server logfile";
          LOGERROR(result);
          return result;          
        }
        pos->close();
        fos->close();
        fout.deleteFile();
        return result;
      }

      PresetVerifier::PresetVerifier() {
      }

      PresetVerifier::~PresetVerifier() {
      }
    }
  }
}
