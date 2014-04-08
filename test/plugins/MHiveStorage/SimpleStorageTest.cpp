#include "SimpleStorageTest.h"

#include "org/esb/av/FormatBaseStream.h"
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/hive/Environment.h"

#include "org/esb/core/StorageEngine.h"
#include "org/esb/util/Log.h"

#include "org/esb/db/Unit.h"
#include "org/esb/db/Profile.h"

#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/util/Foreach.h"


#include "org/esb/hive/DatabaseService.h"

using org::esb::hive::Environment;
using org::esb::core::StorageEngine;
using org::esb::model::Profile;
using org::esb::model::Job;
using org::esb::hive::job::ProcessUnit;
using org::esb::hive::DatabaseService;
using org::esb::model::OutputFile;
bool testProcessUnit(StorageEngine * engine){
  /*testing processunit storage*/
  boost::shared_ptr<ProcessUnit> punit(new ProcessUnit());
  punit->setJobId("0815");
  punit->uuid="4711";
  engine->enque(punit);

  boost::shared_ptr<ProcessUnit> punit2=engine->deque();
  assert(punit2->getJobId()=="0815");
  assert(punit2->uuid=="4711");

  boost::shared_ptr<ProcessUnit> punit3=engine->deque();
  assert(!punit3);

  engine->rollback(punit2->uuid);

  boost::shared_ptr<ProcessUnit> punit4=engine->deque();
  assert(punit4);
  assert(punit4->uuid=="4711");
  return true;
}

bool testProfile(StorageEngine * engine){
  Profile profile;
  profile.data="{data:1}";
  profile.name="test-name";
  profile.uuid="0815";

  /*create*/
  engine->putProfile(profile);
  LOGDEBUG("profile.uuid="<<profile.uuid);

  /*get*/
  Profile profile2=engine->getProfileByUUID(profile.uuid);
  assert(profile.uuid==profile2.uuid);

  /*update*/
  profile2.name="updated-name";
  engine->putProfile(profile2);

  /*list*/
  std::list<Profile>list=engine->getProfileList();
  foreach(Profile profile,list){
    LOGDEBUG("Profile.uuid="<<profile.uuid<<" name = "<<profile.name);
  }

  /*delete*/
  engine->deleteProfile(list.front());
  std::list<Profile>list2=engine->getProfileList();
  assert(list2.size()==list.size()-1);

  return true;
}

bool testOutputFile(StorageEngine * engine){
  OutputFile outfile;
  outfile.jobid="0815";
  outfile.uuid="4711";


  engine->putOutputFile(outfile);
  OutputFile outfile2=engine->getOutputFileByUUID(outfile.uuid);

  assert(outfile2.uuid==outfile.uuid);

  std::list<OutputFile>list=engine->getOutputFileList();
  assert(list.size()==1);

return true;
}


bool testJob(StorageEngine * engine){
  Job job;
  job.graph="graph";
  job.status="test";
  job.uuid="4711";

  engine->putJob(job);

  Job job2=engine->getJobByUUID(job.uuid);

  assert(job2.uuid==job.uuid);

  return true;
}

int main(int argc, char ** argv){

  Environment::set("MHIVE_DATA_PATH",".");
  Environment::build(argc, argv);
  Environment::set("storage.engine","sqlite3");
  Log::open("");

  /*cleanup test environment*/
  DatabaseService::dropTables();
  DatabaseService::createDatabase();
  DatabaseService::createTables();

  org::esb::av::FormatBaseStream::initialize();
  org::esb::core::PluginRegistry::getInstance()->load(STORAGE_PLUGIN);

  org::esb::core::PluginRegistry::getInstance()->initPlugins();

  StorageEngine * engine=org::esb::core::PluginRegistry::getInstance()->getStorageEngine();
  assert(engine);
  assert(testProcessUnit(engine));
  assert(testProfile(engine));
  assert(testOutputFile(engine));
  assert(testJob(engine));

  //engine->writeUnitStream();

  return 0;
}
