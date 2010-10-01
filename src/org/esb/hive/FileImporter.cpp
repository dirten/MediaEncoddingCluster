#ifndef IMPORT_CPP
#define IMPORT_CPP
//#include <iostream>
//#include <fstream>
#include <map>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include "org/esb/util/Decimal.h"
//#include "org/esb/hive/DatabaseUtil.h"
//#include <boost/progress.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/polymorphic_binary_iarchive.hpp> 
//#include <boost/archive/polymorphic_binary_oarchive.hpp> 

#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/config/config.h"
//#include "tntdb/connect.h"
//#include "tntdb/connection.h"
//#include "tntdb/statement.h"
//#include "tntdb/blob.h"
//#include "org/esb/sql/sqlite3x.hpp"
#include "org/esb/hive/DatabaseService.h"
#include "FileImporter.h"
using namespace std;
using namespace org::esb;
using namespace org::esb::config;
//using namespace tntdb;
using namespace org::esb::av;
//using namespace boost;
//using namespace sqlite3x;

string toString(int num) {
  char c[10];
  memset(&c, 0, 10);
  sprintf(c, "%d", num);
  return string(c);
}

template < class T > T nullCheck(T param) {
  return param;
}

struct stream_group {
  long long int id; // database stream id
  int idx; // file stream index
  long long int start_ts; // group start ts
  int packet_count; // group packet count
  int stream_type; // audio or video type
  int frame_group; // ???
};

/*
int import(org::esb::io::File file) {
  std::string name = file.getPath();
  char * argv[] = {const_cast<char*>(""), const_cast<char*>(name.c_str())};

//  import(2, argv);
   return 1;
}
 */

namespace org {
  namespace esb {
    namespace hive {
      

      FileImporter::FileImporter() {
        if (!_connection) {
          _connection = new db::HiveDb(org::esb::hive::DatabaseService::getDatabase());
        }

      }

      db::MediaFile FileImporter::import(org::esb::io::File file) {
        if (!file.exists() || !file.canRead()) {
          LOGERROR("Source File not found:" << file.getPath());
        } else {
          LOGDEBUG("File:" << file.getPath());
        }
        
        db::MediaFile mediafile(*_connection);
        FormatInputStream fis(&file);
        if (!fis.isValid())return mediafile;
        PacketInputStream pis(&fis);
        int id = 0;
        //  try {
        //    db::MediaFile mfile=litesql::select<db::MediaFile>(db, db::MediaFile::Path==file.getFilePath() && db::MediaFile::Filename==file.getFileName()).one();
        //    id=mfile.id;
        //  } catch (litesql::NotFound ex) {

        mediafile.filename = file.getFileName();
        mediafile.path = file.getFilePath();
        mediafile.filesize = (double) fis.getFileSize();
        mediafile.streamcount = fis.getStreamCount();
        mediafile.containertype = fis.getFormatContext()->iformat->name;
        mediafile.duration = (double) fis.getFormatContext()->duration;
        mediafile.starttime = (double) fis.getFormatContext()->start_time;
        mediafile.bitrate = fis.getFormatContext()->bit_rate;
        mediafile.update();

        AVFormatContext *ctx = fis.getFormatContext();

        for (int a = 0; a < mediafile.streamcount; a++) {
          db::Stream stream(mediafile.getDatabase());
          stream.streamindex = (int) a;
          stream.streamtype = (int) ctx->streams[a]->codec->codec_type;
          stream.codecid = (int) ctx->streams[a]->codec->codec_id;
          stream.codecname = (const char*) ctx->streams[a]->codec->codec_name;
          stream.frameratenum = ctx->streams[a]->r_frame_rate.num;
          stream.framerateden = ctx->streams[a]->r_frame_rate.den;
          stream.firstpts = (double) ctx->streams[a]->start_time;
          stream.firstdts = (double) ctx->streams[a]->first_dts;
          stream.duration = (double) ctx->streams[a]->duration;
          stream.nbframes = (double) ctx->streams[a]->nb_frames;
          stream.streamtimebasenum = ctx->streams[a]->time_base.num;
          stream.streamtimebaseden = ctx->streams[a]->time_base.den;
          stream.codectimebasenum = ctx->streams[a]->codec->time_base.num;
          stream.codectimebaseden = ctx->streams[a]->codec->time_base.den;
          stream.ticksperframe = ctx->streams[a]->codec->ticks_per_frame;
          stream.width = ctx->streams[a]->codec->width;
          stream.height = ctx->streams[a]->codec->height;
          stream.gopsize = ctx->streams[a]->codec->gop_size;
          stream.pixfmt = (int) ctx->streams[a]->codec->pix_fmt;
          stream.bitrate = ctx->streams[a]->codec->bit_rate;
          stream.samplerate = ctx->streams[a]->codec->sample_rate;
          stream.samplefmt = (int) ctx->streams[a]->codec->sample_fmt;
          stream.channels = ctx->streams[a]->codec->channels;
          stream.bitspercodedsample = ctx->streams[a]->codec->bits_per_coded_sample;
          stream.extradatasize = ctx->streams[a]->codec->extradata_size;
          if (stream.extradatasize > 0)
            stream.extradata = (const char*) (ctx->streams[a]->codec->extradata);
          stream.update();
          mediafile.streams().link(stream);
        }
        return mediafile;

      }
    }
  }
}

#endif
