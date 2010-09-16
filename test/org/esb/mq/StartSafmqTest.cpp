/* 
 * File:   StartSafmqTest.cpp
 * Author: HoelscJ
 *
 * Created on 16. September 2010, 16:28
 */
#define UUID_USE_INTTYPES
#include <cstdlib>
#include "param_config.h"
#include "Log.h"
#include "QManager.h"
#include "ServerThread.h"
#include "SystemDelivery.h"
#include "ForwardThread.h"
using namespace std;

/*
 * 
 */
#define STARTUP_LOG "startup_log"
#define SHUTDOWN_LOG    "shutdown_log"
#define SIGNON_LOG              "signon_log"
#define USER_LOG                "user_log"
#define GROUP_LOG               "group_log"
#define QUEUE_LOG               "queue_log"
#define MESSAGE_LOG             "message_log"
#define FORWARD_LOG             "forward_log"
#define INFO_LOG                "info_log"
#define FORWARD_THREADS         "forward_threads"
Config * pcfg = NULL;
QManager * theQueueManager = NULL;
std::string system_user;
std::string safmq_all_users;

int main(int argc, char** argv) {
  /// The global instance of the network serevr thread
  ServerThread server;
  /// The global instance of the TTL Error Delivery thread
  SystemDelivery sysdeliver;
  /// The global instance of the message forwarding thread
  ForwardThread* forwardthds;
  int numForwardThreads = 1;

  try {
    pcfg = new Config("test.cfg");
  } catch (int) {
    safmq::Log::getLog()->Startup(safmq::Log::error, "Unable to load configuration file.");
    return -1;
  }

  // setup up the logger
  safmq::Log* plog = safmq::Log::getLog();

  plog->SetLogDestination(safmq::Log::startup, pcfg->getParam(STARTUP_LOG, safmq::Log::STDOUT).c_str());
  plog->SetLogDestination(safmq::Log::shutdown, pcfg->getParam(SHUTDOWN_LOG, safmq::Log::STDOUT).c_str());
  plog->SetLogDestination(safmq::Log::sign_on, pcfg->getParam(SIGNON_LOG, safmq::Log::STDOUT).c_str());
  plog->SetLogDestination(safmq::Log::user, pcfg->getParam(USER_LOG, safmq::Log::STDOUT).c_str());
  plog->SetLogDestination(safmq::Log::group, pcfg->getParam(GROUP_LOG, safmq::Log::STDOUT).c_str());
  plog->SetLogDestination(safmq::Log::queue, pcfg->getParam(QUEUE_LOG, safmq::Log::STDOUT).c_str());
  plog->SetLogDestination(safmq::Log::message, pcfg->getParam(MESSAGE_LOG, safmq::Log::STDOUT).c_str());
  plog->SetLogDestination(safmq::Log::forward, pcfg->getParam(FORWARD_LOG, safmq::Log::STDOUT).c_str());
  plog->SetLogDestination(safmq::Log::info, pcfg->getParam(INFO_LOG, safmq::Log::STDOUT).c_str());

  theQueueManager = new QManager;

  //atexit(onExit);

  sysdeliver.start();
  theQueueManager->start();


  numForwardThreads = pcfg->getIntParam(FORWARD_THREADS, 1);
  if (numForwardThreads < 1)
    numForwardThreads = 1;
  forwardthds = new ForwardThread[numForwardThreads];
  for (int x = 0; x < numForwardThreads; ++x) {
    forwardthds[x].start();
  }
  server.start();
#ifdef SAFMQ_SSL
  sslserver.start();
#endif

  plog->Startup();
  server.join();
  for (int x = 0; x < numForwardThreads; ++x)
    forwardthds[x].join();
  delete [] forwardthds;
  sysdeliver.join();
  theQueueManager->join();

  //Log::getLog()->Info("Shutting down SAFMQ, freeing theQueueManager");
  //delete theQueueManager;

  Log::getLog()->Shutdown();

  return 0;
}

