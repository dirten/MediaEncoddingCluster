/* 
 * File:   MHiveMaster.cpp
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 16:11
 */

#include <cstdlib>
#include "org/esb/mq/QueueManager.h"
#include "org/esb/lang/CtrlCHitWaiter.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  org::esb::mq::QueueManager man;
  man.start();
  org::esb::lang::CtrlCHitWaiter::wait();
  man.stop();

  return 0;
}

