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
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/config/config.h"
#include <inttypes.h>
#include "param_config.h"
#include "QManager.h"
#include "Log.h"
#include "SystemDelivery.h"
#include "ServerThread.h"
#include "SystemDelivery.h"
#include "ForwardThread.h"
Config * pcfg = NULL;
QManager * theQueueManager = NULL;
/// The global instance of the network serevr thread
ServerThread* server;
/// The global instance of the TTL Error Delivery thread
SystemDelivery* sysdeliver;
/// The global instance of the message forwarding thread
ForwardThread* forwardthds;
safmq::Log* plog;

const std::string _system_user("safmq_system");
//const std::string safmq_all_users("safmq_all_users");

/// Global group including all users
const std::string _safmq_all_users("safmq_all_users");
namespace org {
  namespace esb {
    namespace mq {

      QueueManager::QueueManager(std::string base_dir) {
        sysdeliver = new SystemDelivery();
        server = new ServerThread();
        org::esb::io::File file(base_dir + "/queues");
        if (!file.exists()) {
          file.mkdirs();
        }
        org::esb::io::File f(base_dir + "/mq.cfg");
        //if(!f.exists()){
        org::esb::io::FileOutputStream fos(&f);
        std::string line;
        line = "port:";
        line += "20202\n";
        fos.write(line);
        line = "queue_dir:";
        line += file.getFilePath() + "\n";
        fos.write(line);
        fos.close();
        //}
        numForwardThreads = 1;
        _running = false;
        try {
          pcfg = new Config(base_dir + "/mq.cfg");

          //          std::string port=pcfg->getParam("port");
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
        theQueueManager = NULL;
        plog->Shutdown();
        delete plog;
        plog = NULL;
        delete pcfg;
        pcfg = NULL;
      }

      void QueueManager::createQueue(std::string name) {
      }

      std::string QueueManager::getUrl() {
        std::string url = "safmq://admin:@localhost:";
        url += pcfg->getParam("port", "9000");
        return url;
      }

      void QueueManager::start() {
        sysdeliver->start();
        theQueueManager->start();


        numForwardThreads = pcfg->getIntParam(FORWARD_THREADS, 1);
        if (numForwardThreads < 1)
          numForwardThreads = 1;
        forwardthds = new ForwardThread[numForwardThreads];
        for (int x = 0; x < numForwardThreads; ++x) {
          forwardthds[x].start();
        }
        server->start();
        //plog->Startup();
        _running = true;

      }

      void QueueManager::stop() {
        if (!_running)return;
        server->stop();
        for (int x = 0; x < numForwardThreads; ++x)
          forwardthds[x].stop();
        sysdeliver->stop();
        if (theQueueManager)
          theQueueManager->stop();
        server->join();
        for (int x = 0; x < numForwardThreads; ++x)
          forwardthds[x].join();
        delete [] forwardthds;
        sysdeliver->join();
        delete sysdeliver;
        theQueueManager->join();

        //Log::getLog()->Info("Shutting down SAFMQ, freeing theQueueManager");
        //delete theQueueManager;


        _running = false;
      }
    }
  }
}
