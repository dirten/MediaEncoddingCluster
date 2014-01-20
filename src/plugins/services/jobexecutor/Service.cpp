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
//#include "org/esb/hive/HiveClient.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/core/Graph.h"
#include "org/esb/core/GraphParser.h"
#include "org/esb/core/GraphException.h"
#include "org/esb/config/config.h"
#include "Poco/File.h"
namespace jobexecutor {

  Service::Service() {
    _status = NONE;
    org::esb::signal::Messenger::getInstance().addMessageListener(*this);

    //LOGDEBUG2("bla fasel, test log");
  }

  Service::~Service() {
  }

  void Service::onMessage(org::esb::signal::Message &msg) {
    if(msg.containsProperty("jobexecutor")){
      if (msg.getProperty<std::string > ("jobexecutor") == "STOP_JOB") {
        LOGDEBUG("STOP_JOB request try cancel graph");
        _current_graph.cancel();
      }

    }
  }

  void Service::init() {

  }

  void Service::startService() {
    /*clean up interrupted encodings*/
    LOGDEBUG("looking for aborted Jobs");
    litesql::DataSource<db::Job> source = litesql::select<db::Job > (*getContext()->database, db::Job::Status == db::Job::Status::Waiting).orderBy(db::Job::Id, false);
    litesql::Cursor<db::Job> cur=source.cursor();
    for (;cur.rowsLeft(); cur++){
      db::Job out=(*cur);
      LOGDEBUG("aborted job found, finishing it:"<<out)
      out.status=db::Job::Status::Exporting;
      out.update();
      LOGDEBUG("changed to:"<<out)
    }
    return;

    if (source.count() > 0) {
      db::Job job = source.one();
      LOGDEBUG("aborted encoding found:" << job.uuid);
      job.status = db::Job::Status::Waiting;
      job.update();

      std::string base = org::esb::config::Config::get("hive.tmp_path");
      Poco::File jobdir(base + "/jobs/" + job.uuid.value());
      if (jobdir.exists()) {
        LOGDEBUG("delete the previous created job directory");
        jobdir.remove(true);
      }

    }

    _status = RUNNING;
    go(Service::run, this);
  }

  void Service::stopService() {
    _status = STOPPING;
  }

  void Service::actualizeProgress(org::esb::core::Graph * graph) {
    //while(graph->getState()!=org::esb::core::Graph::DONE&&graph->getState()!=org::esb::core::Graph::ERROR){
    LOGDEBUG("Reading Graph Progress");
    int gprogress = (graph->getProcessedStepCount()*100) / graph->getStepCount();
    _job->graphstatus = graph->getStatus();
    //std::cout<<graph->getStatus()<<std::endl;
    _job->progress = gprogress;
    _job->update();
    //std::cout<<*_job<<std::endl;
    //org::esb::lang::Thread::sleep2(1000);
    //}
  }

  void Service::run() {
    //LOGTRACEMETHOD("void JobExecutor::run() ")
    while (_status == RUNNING) {
      litesql::DataSource<db::Job> source = litesql::select<db::Job > (*getContext()->database, db::Job::Status == db::Job::Status::Waiting);
      if (source.count() > 0) {
        //LOGDEBUG("New Job found!!!");
        db::Job job = source.one();
        std::string graphdata = job.graph;
        try {

          org::esb::core::GraphParser graphparser(graphdata);
          org::esb::core::GraphParser::ElementMap & el = graphparser.getElementMap();
          std::list<Ptr<org::esb::core::Graph::Element> > list;

          foreach(org::esb::core::GraphParser::ElementMap::value_type & element, el) {
            element.second->task->getContext()->set<std::string>("uuid",job.uuid);
            list.push_back(element.second);
          }
          _job = &job;
          _current_graph=org::esb::core::Graph(list, job.uuid);
          //_current_graph=graph;
          _current_graph.addStatusObserver(boost::bind(&Service::actualizeProgress, this, _1));
          //go(Service::actualizeProgress, this, &graph,job);

          job.status = job.Status.Processing;
          job.update();
          /**
           * need to simply walk/execute the graph here and not int the graph class,
           * this should be needed to keep the graph class clean from any database access
           */
          _current_graph.run();
          //actualizeProgress(&graph, job);
          if (_current_graph.getState() == org::esb::core::Graph::DONE)
            job.status = job.Status.Completed;
          if (_current_graph.getState() == org::esb::core::Graph::DONE_WITH_ERROR)
            job.status = job.Status.CompletedWithError;
          job.update();
        } catch (org::esb::core::GraphException & ex) {
          job.status = job.Status.Error;
          job.update();
          db::JobLog log(job.getDatabase());
          log.message = ex.what();
          log.update();
          job.joblog().link(log);
          LOGERROR(ex.what())

        } catch (std::exception & ex) {
          job.status = job.Status.Error;
          job.update();
          db::JobLog log(job.getDatabase());
          log.message = ex.what();
          log.update();
          job.joblog().link(log);
          LOGERROR(ex.what())
        }

        if (job.status != job.Status.Error)
          job.status = job.Status.Completed;
        job.update();
      } else {
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
