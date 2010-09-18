/* 
 * File:   QManager.cpp
 * Author: HoelscJ
 * 
 * Created on 17. September 2010, 11:49
 */
/*this is nedded to run the SafmqManager embedded in the mhive*/
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


#include "QueueManager.h"
#include "org/esb/io/File.h"
Config * pcfg = NULL;
QManager * theQueueManager = NULL;
const std::string			system_user("safmq_system");

/// Global group including all users
const std::string			safmq_all_users("safmq_all_users");
namespace org {
  namespace esb {
    namespace mq {

      QueueManager::QueueManager() {
        org::esb::io::File file("queues");
        if(!file.exists()){
          file.mkdir();
        }
        numForwardThreads = 1;
        _running=false;
        try {
          pcfg = new Config("test.cfg");
        } catch (int) {
          safmq::Log::getLog()->Startup(safmq::Log::error, "Unable to load configuration file.");
        }
        plog = safmq::Log::getLog();
        /*
        plog->SetLogDestination(safmq::Log::startup, pcfg->getParam(STARTUP_LOG, safmq::Log::STDOUT).c_str());
        plog->SetLogDestination(safmq::Log::shutdown, pcfg->getParam(SHUTDOWN_LOG, safmq::Log::STDOUT).c_str());
        plog->SetLogDestination(safmq::Log::sign_on, pcfg->getParam(SIGNON_LOG, safmq::Log::STDOUT).c_str());
        plog->SetLogDestination(safmq::Log::user, pcfg->getParam(USER_LOG, safmq::Log::STDOUT).c_str());
        plog->SetLogDestination(safmq::Log::group, pcfg->getParam(GROUP_LOG, safmq::Log::STDOUT).c_str());
        plog->SetLogDestination(safmq::Log::queue, pcfg->getParam(QUEUE_LOG, safmq::Log::STDOUT).c_str());
        plog->SetLogDestination(safmq::Log::message, pcfg->getParam(MESSAGE_LOG, safmq::Log::STDOUT).c_str());
        plog->SetLogDestination(safmq::Log::forward, pcfg->getParam(FORWARD_LOG, safmq::Log::STDOUT).c_str());
        plog->SetLogDestination(safmq::Log::info, pcfg->getParam(INFO_LOG, safmq::Log::STDOUT).c_str());
*/
        theQueueManager = new QManager;

        //atexit(onExit);


      }

      QueueManager::QueueManager(const QueueManager& orig) {
      }

      QueueManager::~QueueManager() {
        stop();
        delete theQueueManager;
        theQueueManager=NULL;
        delete plog;
        plog=NULL;
        delete pcfg;
        pcfg=NULL;
      }

      void QueueManager::start() {
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
        plog->Startup();
        _running=true;

      }

      void QueueManager::stop() {
        if(!_running)return;
        server.stop();
        for (int x = 0; x < numForwardThreads; ++x)
          forwardthds[x].stop();
        sysdeliver.stop();
        if (theQueueManager)
          theQueueManager->stop();
        server.join();
        for (int x = 0; x < numForwardThreads; ++x)
          forwardthds[x].join();
        delete [] forwardthds;
        sysdeliver.join();
        theQueueManager->join();

        //Log::getLog()->Info("Shutting down SAFMQ, freeing theQueueManager");
        //delete theQueueManager;

        Log::getLog()->Shutdown();
        _running=false;
      }
    }
  }
}
