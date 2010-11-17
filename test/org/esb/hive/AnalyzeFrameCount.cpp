/* 
 * File:   AnalyzeFrameCount.cpp
 * Author: HoelscJ
 *
 * Created on 17. November 2010, 14:08
 */

#include <cstdlib>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/hive/FileExporter.h"
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  if(argc<3)
    std::cout << "usage : AnalyzeFrameCount basepath<string> streamid<int> "<<std::endl;
  std::string path=argv[1];
  int sid=atoi(argv[2]);

  org::esb::config::Config::setProperty("hive.base_path",path);
  org::esb::hive::DatabaseService::start(path);
  db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();



  litesql::Cursor<db::ProcessUnit> units = litesql::select<db::ProcessUnit > (db, db::ProcessUnit::Targetstream==sid).cursor();

  //std::string path = org::esb::config::Config::getProperty("hive.base_path");
  path += "/tmp/";
    int i=0;
    int64_t count_inframes=0;
    int64_t count_outframes=0;
    double count_diff=0;

  for (; units.rowsLeft(); units++) {
    db::ProcessUnit unit = (*units);

    int pu_id = unit.id;
    //      logdebug("open PU with id : " << pu_id)
    std::string name = path;
    name += org::esb::util::StringUtil::toString(pu_id % 10);
    name += "/";
    name += org::esb::util::StringUtil::toString(pu_id);
    name += ".unit";
    org::esb::io::File infile(name.c_str());
    if (!infile.exists()) {
      LOGERROR(infile.getFileName() << ": not found, this may lead in a resulting audio/video desync");
      continue;
    }
    LOGDEBUG("reading archive # " << pu_id);

    org::esb::io::FileInputStream fis(&infile);
    org::esb::io::ObjectInputStream ois(&fis);
    org::esb::hive::job::ProcessUnit pu;
    if (ois.readObject(pu) != 0) {
      LOGERROR("reading archive # " << pu_id);
      continue;
    }
    count_inframes+=pu._input_packets.size();
    count_outframes+=pu._output_packets.size();
    count_diff+=pu._frameRateCompensateBase;
    LOGDEBUG("inframes:"<<pu._input_packets.size()<<" outframes:"<<pu._output_packets.size()<<" drift:"<<pu._frameRateCompensateBase << " expected:"<<pu._expected_frame_count);
    LOGDEBUG("countinframes:"<<count_inframes<<" countoutframes:"<<count_outframes<<" countdrift:"<<count_diff );
    if(pu._output_packets.size()!=pu._expected_frame_count){
      LOGDEBUG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
      i+=pu._output_packets.size()-pu._expected_frame_count;
    }
    LOGDEBUG("Frames Diff Global "<<i);
  }


  return 0;
}

