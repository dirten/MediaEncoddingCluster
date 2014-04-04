#include "SimpleStorageTest.h"

#include "org/esb/av/FormatBaseStream.h"
#include "org/esb/core/PluginRegistry.h"
#include "org/esb/hive/Environment.h"

#include "org/esb/core/StorageEngine.h"
#include "org/esb/util/Log.h"

using org::esb::hive::Environment;
using org::esb::core::StorageEngine;

int main(int argc, char ** argv){

  Environment::build(argc, argv);
  Environment::set("storage.engine","sqlite3");
  Log::open("");

  org::esb::av::FormatBaseStream::initialize();
  org::esb::core::PluginRegistry::getInstance()->load(STORAGE_PLUGIN);

  org::esb::core::PluginRegistry::getInstance()->initPlugins();

  StorageEngine * engine=org::esb::core::PluginRegistry::getInstance()->getStorageEngine();
  assert(engine);

  //engine->writeUnitStream();

  return 0;
}
