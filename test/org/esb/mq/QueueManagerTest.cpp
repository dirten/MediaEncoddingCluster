/* 
 * File:   QueueManagerTest.cpp
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 13:06
 */

#include <cstdlib>
#include "org/esb/mq/QueueManager.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
using namespace std;

using namespace org::esb::mq;
using namespace org::esb::lang;
/*
 * 
 */
int main(int argc, char** argv) {
  QueueManager man;
  man.start();
  CtrlCHitWaiter::wait();
  man.stop();
  return 0;
}

