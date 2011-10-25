/* 
 * File:   Service.cpp
 * Author: HoelscJ
 * 
 * Created on 10. Oktober 2011, 11:30
 */
#include "org/esb/db/hivedb.hpp"
#include "org/esb/core/PluginContext.h"
#include "org/esb/core/PluginRegistry.h"

#include "Service.h"
#include "org/esb/lang/Process.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/hive/HiveClientAudio.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringTokenizer.h"
namespace clientcontroller {

  Service::Service() {
    _status = NONE;
  }

  Service::~Service() {
  }

  void Service::onMessage(org::esb::signal::Message &msg) {

  }

  void Service::init() {

  }

  void Service::startService() {
    _status = RUNNING;
    go(Service::run, this);
  }

  void Service::stopService() {
    _status = STOPPING;
  }

  void Service::run() {
    LOGTRACEMETHOD("void ProcessUnitController::start() ")
    while (_status == RUNNING) {
      litesql::DataSource<db::Job> source = litesql::select<db::Job > (*getContext()->database, db::Job::Endtime <= 1 && (db::Job::Status == db::Job::Status::Waiting || db::Job::Status == db::Job::Status::Processing));
      if (source.count() > 0) {
        db::Job job = source.one();
        if (job.tasks().get().count() > 0) {
          std::vector<db::Task> tasks = job.tasks().get().all();
          std::map<std::string, std::string> cfg;

          foreach(db::Task & dbtask, tasks) {
            LOGDEBUG("Executing Task : " << dbtask.name << " with parameter : " << dbtask.parameter);
            org::esb::util::StringTokenizer tok(dbtask.parameter, ";");
            while (tok.hasMoreTokens()) {
              std::string line = tok.nextToken();
              org::esb::util::StringTokenizer tok2(line, "=");
              if (tok2.countTokens() == 2) {
                std::string key = tok2.nextToken();
                std::string val = tok2.nextToken();
                cfg[key] = val;
                LOGDEBUG("Setting plugin Context : " << key << "=" << val);
              } else {
                LOGERROR("line : " << line);
              }
            }
            _current_task = org::esb::core::PluginRegistry::getInstance()->createTask(dbtask.name, cfg);
            _current_task->prepare();
            _current_task->execute();
            _current_task->cleanup();
            dbtask.progress=100;
            dbtask.status=dbtask.Status.Complete;
            dbtask.update();
          }
        }
        job.status=job.Status.Completed;
        job.update();
      }
      //else{
      org::esb::lang::Thread::sleep2(getContext()->getEnvironment<int>("jobexecutor.intervall")*1000);
      //}
    }
    _status = STOPPED;
  }

  org::esb::core::OptionsDescription Service::getOptionsDescription() {
    org::esb::core::OptionsDescription result("jobexecutor");
    result.add_options()
            ("jobexecutor.intervall", boost::program_options::value<int>()->default_value(5), "interval in seconds to look for new Jobs");
    return result;
  }

  org::esb::core::ServicePlugin::ServiceType Service::getServiceType() {
    return SERVICE_TYPE_SERVER;
  }
}
