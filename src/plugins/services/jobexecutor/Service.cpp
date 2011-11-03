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
  void Service::actualizeProgress(Ptr<org::esb::core::Task> task,db::Task&dbtask){
    while(task->getStatus()!=org::esb::core::Task::DONE&&task->getStatus()!=org::esb::core::Task::ERROR){
      LOGDEBUG("Reading Progress");
      dbtask.progress=task->getProgress();
      dbtask.update();      
      org::esb::lang::Thread::sleep2(1000);
    }
  }

  void Service::run() {
    LOGTRACEMETHOD("void ProcessUnitController::start() ")
    while (_status == RUNNING) {
      litesql::DataSource<db::Job> source = litesql::select<db::Job > (*getContext()->database, db::Job::Status == db::Job::Status::Waiting);
      if (source.count() > 0) {
        LOGDEBUG("New Job found!!!");
        db::Job job = source.one();
        if (job.tasks().get().count() > 0) {
          int taskcount=job.tasks().get().count();
          int counter=0;
          std::vector<db::Task> tasks = job.tasks().get().all();
          std::map<std::string, std::string> cfg;
          job.status=job.Status.Processing;
          job.update();
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
            try{
            _current_task = org::esb::core::PluginRegistry::getInstance()->createTask(dbtask.name, cfg);
            _current_task->getContext()->_props["job"]=job;
            go(Service::actualizeProgress, this, _current_task,dbtask);
            _current_task->prepare();
              dbtask.status=dbtask.Status.Processing;
              dbtask.update();
            if(_current_task->getStatus()==org::esb::core::Task::ERROR){
              dbtask.status=dbtask.Status.Error;
              dbtask.statustext=_current_task->getStatusMessage();
              dbtask.update();
              break;
            }
            _current_task->execute();
            if(_current_task->getStatus()==org::esb::core::Task::ERROR){
              
              dbtask.status=dbtask.Status.Error;
              dbtask.statustext=_current_task->getStatusMessage();
              dbtask.update();
              break;
            }
            _current_task->cleanup();
            if(_current_task->getStatus()==org::esb::core::Task::ERROR){
              dbtask.status=dbtask.Status.Error;
              dbtask.statustext=_current_task->getStatusMessage();
              dbtask.update();
              break;
            }
            dbtask.progress=100;
            dbtask.status=dbtask.Status.Complete;
            dbtask.update();
            job.progress=(++counter)*100/taskcount;
            job.update();
            }catch(std::exception & ex){
            //dbtask.progress=0;
            dbtask.status=dbtask.Status.Error;
            dbtask.statustext=ex.what();
            dbtask.update();
            
            job.status=job.Status.Error;
            job.update();
            break;
              
            }
          }
        }
        job.status=job.Status.Completed;
        job.update();
      }else{
        LOGDEBUG("no new job found");
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
