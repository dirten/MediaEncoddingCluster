/* 
 * File:   JobCreatorTest.cpp
 * Author: HoelscJ
 *
 * Created on 15. Juli 2010, 18:24
 */

#include <cstdlib>
#include "org/esb/db/hivedb.hpp"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/hive/job/JobController.h"
#include "org/esb/io/File.h"
#include "org/esb/hive/JobUtil.h"
#include "org/esb/hive/FileImporter.h"

#include "config.h"
using namespace std;
using namespace org::esb;

/*
 * 
 */

void assertStreams(db::Stream a, db::Stream b) {
  assert(a.id == b.id);
  assert(a.streamtype == b.streamtype);
  assert(a.streamindex == b.streamindex);
  assert(a.codecid == b.codecid);
  assert(a.frameratenum == b.frameratenum);
  assert(a.framerateden == b.framerateden);
  assert(a.streamtimebasenum == b.streamtimebasenum);
  assert(a.streamtimebaseden == b.streamtimebaseden);
  assert(a.codectimebasenum == b.codectimebasenum);
  assert(a.codectimebaseden == b.codectimebaseden);
  assert(a.firstpts == b.firstpts);
  assert(a.firstdts == b.firstdts);
  assert(a.duration == b.duration);
  assert(a.nbframes == b.nbframes);
  assert(a.ticksperframe == b.ticksperframe);
  assert(a.framecount == b.framecount);
  assert(a.width == b.width);
  assert(a.height == b.height);
  assert(a.gopsize == b.gopsize);
  assert(a.pixfmt == b.pixfmt);
  assert(a.bitrate == b.bitrate);
  assert(a.samplerate == b.samplerate);
  assert(a.channels == b.channels);
  assert(a.bitspercodedsample == b.bitspercodedsample);
  assert(a.flags == b.flags);
  assert(a.extraprofileflags == b.extraprofileflags);


}

int main(int argc, char** argv) {
  Log::open("");

  std::string host = "host=";
  host += DEFAULT_DATABASE_HOST;
  host += ";user=root;port=3306;database=example";
  config::Config::setProperty("db.url", host.c_str());



  std::string src = MEC_SOURCE_DIR;
  src.append("/test.dvd");
  hive::DatabaseService::start(MEC_SOURCE_DIR);
  {
    if (!hive::DatabaseService::databaseExist()) {
      hive::DatabaseService::createDatabase();
    }
    hive::DatabaseService::dropTables();
    hive::DatabaseService::updateTables();
    hive::DatabaseService::loadPresets();

    int fileid = import(org::esb::io::File(src));
    assert(fileid > 0);
    int jobid = jobcreator(fileid, 1, "/tmp");
    assert(jobid > 0);


    db::HiveDb db("mysql", org::esb::config::Config::getProperty("db.url"));
    //db.verbose = true;

    db::Job job = litesql::select<db::Job > (db, db::Job::Id == jobid).one();

    db::MediaFile infile = job.inputfile().get().one();
    db::MediaFile outfile = job.outputfile().get().one();

    assert(infile.id > 0);
    assert(outfile.id > 0);

    assert(infile.id == 1);
    assert(outfile.id == 2);

    vector<db::JobDetail> details = job.jobdetails().get().all();
    assert(details.size() == 2);


    db::JobDetail detail1 = details[0];

    db::Stream stream1input = detail1.inputstream().get().one();
    db::Stream stream1output = detail1.outputstream().get().one();

    vector<db::Stream> istrs = infile.streams().get().all();
    vector<db::Stream> ostrs = outfile.streams().get().all();

    assertStreams(stream1input, istrs[0]);
    assertStreams(stream1output, ostrs[0]);




    db::JobDetail detail2 = details[1];

    db::Stream stream2input = detail2.inputstream().get().one();
    db::Stream stream2output = detail2.outputstream().get().one();

    assertStreams(stream2input, istrs[1]);
    assertStreams(stream2output, ostrs[1]);


    using namespace org::esb::hive::job;
    JobController ctrl;
    db::Job dbjob = ctrl.getJob();
    dbjob.begintime=0;
    dbjob.update();

    //  std::cout << job << std::endl;








    //hive::DatabaseService::dropDatabase();
  }
  hive::DatabaseService::stop();
  Log::close();
  config::Config::close();

  return 0;
}

