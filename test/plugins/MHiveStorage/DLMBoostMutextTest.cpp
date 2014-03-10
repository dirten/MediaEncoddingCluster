#include "DLMBoostMutextTest.h"
#include "plugins/MHiveStorage/services/engines/redundant_engine/DLMBoostMutex.h"
#include "plugins/MHiveStorage/services/engines/redundant_engine/DLMHost.h"
#include "org/esb/util/Log.h"

DLMBoostMutextTest::DLMBoostMutextTest()
{
}

using mhivestorage::engines::DLMBoostMutex;
using mhivestorage::engines::DLMHost;

int main(int argc, char**argv){
   Log::open("");

   std::vector<std::string> hosts;

   DLMHost  host1(8181);
   DLMHost  host2(8182);
   DLMHost  host3(8183);

  hosts.push_back("localhost:8181");
  hosts.push_back("localhost:8182");
  hosts.push_back("localhost:8183");





  DLMBoostMutex mutex(8180, hosts);
  mutex.lock();

  LOGDEBUG("lock granted");
  mutex.unlock();

  return 0;
}
