
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Thread.h"
int main(int argc, char** argv) {
  Log::open("");
  org::esb::lang::CtrlCHitWaiter::wait();
  LOGDEBUG("shuting down MHiveReader");
  org::esb::lang::Thread::sleep2(1000);
  LOGDEBUG("MHiveReader is down");
  Log::close();
}