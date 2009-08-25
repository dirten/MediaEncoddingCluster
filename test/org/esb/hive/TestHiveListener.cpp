#include "org/esb/hive/HiveListener.h"
#include "org/esb/config/config.h"
#include "org/esb/signal/Message.h"
#include "org/esb/signal/Messenger.h"
using namespace org::esb::hive;
using namespace org::esb::config;
using namespace org::esb::signal;

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
int main(){
  Config::init("");
  Config::setProperty("db.connection","mysql:host=;db=hive;user=;passwd=");
   org::esb::hive::HiveListener hive;
  Messenger::getInstance().addMessageListener(hive);
      Messenger::getInstance().sendMessage(Message().setProperty("hivelistener", org::esb::hive::START));
      ctrlCHitWait();
      Messenger::getInstance().sendMessage(Message().setProperty("hivelistener", org::esb::hive::STOP));

}
