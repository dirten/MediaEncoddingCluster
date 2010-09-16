#include "org/esb/db/hivedb.hpp"
#include "config.h"
#include "org/esb/config/config.h"

#include "org/esb/web/WebServer.h"
#include "org/esb/lang/Thread.h"
#include <boost/thread.hpp>
#include "org/esb/av/AV.h"

#include "org/esb/hive/JobUtil.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/util/Log.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
using namespace org::esb::lang;
using namespace org::esb::web;
using namespace org::esb::config;
using namespace org::esb::hive;

WebServer * w;


int main(int argc, char**argv) {
  Log::open("");
  av_register_all();
  std::string base_path = MEC_SOURCE_DIR;
  Config::setProperty("db.connection", "mysql:host=127.0.0.1;db=hive2;user=root;passwd=");

  std::string host = "host=";
  host += DEFAULT_DATABASE_HOST;
  host += ";user=root;port=3306;database=example";
  Config::setProperty("db.url", host.c_str());

  Config::setProperty("hive.base_path", base_path.c_str());
  org::esb::hive::DatabaseService::start(base_path);

  if (!DatabaseService::databaseExist()) {
    DatabaseService::createDatabase();
  }
  DatabaseService::dropTables();
  DatabaseService::updateTables();
  DatabaseService::loadPresets();
  std::string docroot = base_path.append("/web");
  Config::setProperty("web.docroot", docroot.c_str());
  Config::init("../../mec/.hive");
  {
  db::HiveDb db("mysql",Config::getProperty("db.url"));
  db::MediaFile file(db);
  file.path="/tmp/test/path";
  file.filename="testfile.avi";
  file.update();

  db::Watchfolder w(db);
  
  w.infolder="/video";
  w.outfolder="/tmp";
  
  db::Profile p =litesql::select<db::Profile>(db, db::Profile::Id==1).one();
  w.update();
  w.profile().link(p);
  w.update();
  
  
    std::string src = MEC_SOURCE_DIR;
  src.append("/test.dvd");
  org::esb::hive::FileImporter imp;

      db::MediaFile mediafile = imp.import(org::esb::io::File(src));
    assert(mediafile.id > 0);
      db::Profile pro = litesql::select<db::Profile > (mediafile.getDatabase(), db::Profile::Id == 1).one();

    int jobid = jobcreator(mediafile, pro, "/tmp");
    assert(jobid > 0);

  }
  

  w= new WebServer();
  w->start();

  if(argc>1)
    org::esb::lang::CtrlCHitWaiter::wait();

  w->stop();
  delete w;
  org::esb::hive::DatabaseService::stop();
  Config::close();
  LOGDEBUG("close log");
  Log::close();
}
