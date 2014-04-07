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
using org::esb::hive::Environment;
using org::esb::core::StorageEngine;
using org::esb::model::Profile;
using org::esb::hive::job::ProcessUnit;

bool testProcessUnit(StorageEngine * engine){
  /*testing processunit storage*/
  boost::shared_ptr<ProcessUnit> punit(new ProcessUnit());
  punit->setJobId("0815");
  punit->uuid="4711";
  engine->enque(punit);

  boost::shared_ptr<ProcessUnit> punit2=engine->deque();
  assert(punit2->getJobId()=="0815");
  assert(punit2->uuid=="4711");

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

int main(int argc, char ** argv){

  Environment::set("MHIVE_DATA_PATH",".");
  Environment::build(argc, argv);
  Environment::set("storage.engine","sqlite3");
  Log::open("");

  org::esb::av::FormatBaseStream::initialize();
  org::esb::core::PluginRegistry::getInstance()->load(STORAGE_PLUGIN);

  org::esb::core::PluginRegistry::getInstance()->initPlugins();

  StorageEngine * engine=org::esb::core::PluginRegistry::getInstance()->getStorageEngine();
  assert(engine);
  assert(testProcessUnit(engine));
  assert(testProfile(engine));


  //engine->writeUnitStream();

  return 0;
}
