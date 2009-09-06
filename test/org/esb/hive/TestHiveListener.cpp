#include "org/esb/hive/HiveListener.h"
#include "org/esb/config/config.h"
#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"
#include <boost/thread.hpp>
#include "org/esb/util/Log.h"
using namespace org::esb::hive;
using namespace org::esb::config;
using namespace org::esb::signal;

#ifdef WIN32

boost::mutex terminationMutex;
boost::condition ctrlCHit;
boost::condition serverStopped;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type) {
  switch (ctrl_type) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    {
      boost::mutex::scoped_lock terminationLock(terminationMutex);
      logdebug("ctlc event");
      ctrlCHit.notify_all(); // should be just 1

      //      serverStopped.wait(terminationLock);
      return TRUE;
    }
    default:
      return FALSE;
  }
}

void ctrlCHitWait() {
  SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
  boost::mutex::scoped_lock terminationLock(terminationMutex);
  ctrlCHit.wait(terminationLock);
}
#else
void ctrlCHitWait() {
  sigset_t wait_mask2;
  sigemptyset(&wait_mask2);
  sigaddset(&wait_mask2, SIGINT);
  sigaddset(&wait_mask2, SIGQUIT);
  sigaddset(&wait_mask2, SIGTERM);
  sigaddset(&wait_mask2, SIGCHLD);
  pthread_sigmask(SIG_BLOCK, &wait_mask2, 0);
  int sig = 0;
  //sigdelset(&wait_mask, SIGCHLD);

  int err;
  do {
    err = sigwait(&wait_mask2, &sig);
  } while (err != 0);

}
#endif
int main(){
  Config::init("");
  Config::setProperty("db.connection","mysql:host=;db=hive;user=;passwd=");
   org::esb::hive::HiveListener hive;
//   hive.startListener();
//  Messenger::getInstance().addMessageListener(hive);
//      Messenger::getInstance().sendMessage(Message().setProperty("hivelistener", org::esb::hive::START));
//      ctrlCHitWait();
//      Messenger::getInstance().sendMessage(Message().setProperty("hivelistener", org::esb::hive::STOP));

}
