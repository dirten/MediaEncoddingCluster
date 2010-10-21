/* 
 * File:   CodecFactoryTest.cpp
 * Author: HoelscJ
 *
 * Created on 14. Juli 2010, 19:48
 */

#include <cstdlib>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/config/config.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/io/File.h"

#include "org/esb/av/Decoder.h"
#include "org/esb/av/Encoder.h"
#include "org/esb/hive/CodecFactory.h"
#include "boost/shared_ptr.hpp"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {


  Log::open("");
  std::string src = MEC_SOURCE_DIR;
  org::esb::config::Config::setProperty("hive.base_path", src.c_str());
  org::esb::config::Config::setProperty("db.url", "host=localhost;user=root;port=3306;database=example");

  src.append("/test.dvd");
  org::esb::hive::DatabaseService::start(MEC_SOURCE_DIR);
  if (org::esb::hive::DatabaseService::databaseExist()) {
    org::esb::hive::DatabaseService::dropDatabase();
  }

  org::esb::hive::DatabaseService::createDatabase();
  org::esb::hive::DatabaseService::createTables();
  {
    org::esb::hive::FileImporter imp;
    db::MediaFile mediafile = imp.import(org::esb::io::File(src));
    assert(mediafile.id > 0);



    //mediafile=litesql::select<db::MediaFile>(mediafile.getDatabase(), db::MediaFile::Id==(int)mediafile.id).one();


    vector<db::Stream> streams = mediafile.streams().get().all();

    assert(streams.size() == 2);

    db::Stream stream = streams[0];


    LOGDEBUG(stream);
    boost::shared_ptr<org::esb::av::Decoder>decoder = org::esb::hive::CodecFactory::getStreamDecoder(stream.id);

    assert(decoder->open());
    LOGDEBUG(decoder->toString());
    boost::shared_ptr<org::esb::av::Encoder>encoder = org::esb::hive::CodecFactory::getStreamEncoder(stream.id);


    encoder->setCodecOption("b","1024000");
//    encoder->ctx->rc_buffer_size=3000000;
    LOGDEBUG("VBV="<<encoder->ctx->rc_buffer_size);
    assert(encoder->open());

    LOGDEBUG(encoder->toString());
  }
  org::esb::hive::CodecFactory::free();
  org::esb::hive::DatabaseService::dropDatabase();
  org::esb::hive::DatabaseService::stop();

  return 0;
}

