/* 
 * File:   JobHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 21. Oktober 2011, 14:18
 */

#include "JobHandler.h"
#include "plugins/services/webservice/ServiceRequest.h"
#include "plugins/services/webservice/ServiceResponse.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/core/Task.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "org/esb/util/UUID.h"

namespace jobhandler {

  JobHandler::JobHandler() {

  }

  JobHandler::~JobHandler() {

  }

  void JobHandler::handleRequest(org::esb::core::Request*req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest*sreq = ((org::esb::api::ServiceRequest*) req);
    LOGDEBUG("Url=" << sreq->getRequestURI());
    if (sreq->getRequestURI().find(_base_uri + "/job") == 0) {
      if (sreq->getMethod() == "GET") {
        handleGET(req, res);
      } else if (sreq->getMethod() == "POST") {
        handlePOST(req, res);
      }
    }
  }

  void JobHandler::init() {
    _base_uri = getContext()->getEnvironment<std::string > ("jobhandler.baseuri");
    LOGDEBUG("BaseUri JobHandler=" << _base_uri);
  }

  org::esb::core::OptionsDescription JobHandler::getOptionsDescription() {
    org::esb::core::OptionsDescription result("jobhandler");
    result.add_options()
            ("jobhandler.baseuri", boost::program_options::value<std::string > ()->default_value("/api/v1"), "base uri for the api requests");
    return result;
  }

  bool JobHandler::contains(JSONNode& node, std::string name) {
    bool result = false;
    int size = node.size();
    //LOGDEBUG("NodeSize=" << size);
    if (size > 0) {
      for (int a = 0; a < size; a++) {
        JSONNode n = node[a];
        LOGDEBUG("search for " << name << " iter = " << n.name());
        if (name == n.name()) {
          result = true;
          LOGDEBUG("attribute " << name << " found")
        }
      }
    }
    return result;
  }

  std::string JobHandler::checkJob(JSONNode&root) {
    std::string result;
    if (!contains(root, "tasks")) {
      result = "job has no defined tasks";
    } else if (root["tasks"].size() == 0) {
      result = "job has no defined tasks";
    } else {
      getContext()->database->begin();
      db::Job job(*getContext()->database);

      job.uuid = boost::lexical_cast<std::string > (boost::uuids::random_generator()());
      job.status = db::Job::Status::Waiting;
      job.update();
      int s = root["tasks"].size();
      LOGDEBUG("Array Size=" << s);
      for (int a = 0; a < s; a++) {
        LOGDEBUG("Roundtrip Task" << root["tasks"].at(a).write_formatted());
        std::string r = checkTask(root["tasks"].at(a), job);
        if (r.length() > 0) {
          getContext()->database->rollback();
          return r;
        }
      }
      getContext()->database->commit();
    }
    return result;
  }

  std::string JobHandler::checkTask(JSONNode&root, db::Job & job) {
    std::string result = "";
    if (!contains(root, "name")) {
      result = "task has no defined name";
    } else {
      /*first create an empty named task to resolve the required parameter for it*/
      Ptr<org::esb::core::Task>task = org::esb::core::PluginRegistry::getInstance()->createTask(root["name"].as_string(), std::map<std::string, std::string>());
      if (!task) {
        result = std::string("could not find a definition for task with name ").append(root["name"].as_string());
      } else {
        org::esb::core::OptionsDescription desc = task->getOptionsDescription();
        std::map<std::string, std::string> para;
        typedef boost::shared_ptr<boost::program_options::option_description> option;
        std::string parameter;

        foreach(const option value, desc.options()) {
          std::string def;
          if (!value->semantic()->is_required()) {
            boost::any data;
            value->semantic()->apply_default(data);
            if (data.type() == typeid (int)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<int>(data));
            } else if (data.type() == typeid (double)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<double>(data));
            } else if (data.type() == typeid (bool)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<bool>(data));
            } else if (data.type() == typeid (std::string)) {
              def = org::esb::util::StringUtil::toString(boost::any_cast<std::string > (data));
            } else {
              LOGERROR("Key=" << value->long_name() << " : Unknown value type:" << data.type().name());
            }
            LOGDEBUG("Key=" << value->long_name() << " Default=" << def);
          }
          if (!contains(root, value->long_name()) && (value->semantic()->is_required() || def.length() == 0)) {
            result = std::string("attribute ").append(value->long_name()).append(" for Task ").append(root["name"].as_string()).append(" could not be empty!");
            break;
          }
          std::string key = value->long_name();
          std::string v;
          if(!contains(root, value->long_name())){
            v=def;
          }else{
                v = root[key].as_string();
          }
          parameter += key + "=" + v + ";";
          para[key]=v;
        }
        task = org::esb::core::PluginRegistry::getInstance()->createTask(root["name"].as_string(), para);
        task->getContext()->_props["job"]=job;
        task->prepare();
        if(task->getStatus()==org::esb::core::Task::ERROR){
          result=root["name"].as_string();
          result+=":";
          result+=task->getStatusMessage();
        }
        LOGDEBUG("Task Found: " << root["name"].as_string());
        db::Task t(*getContext()->database);
        t.name = root["name"].as_string();
        t.parameter = parameter;
        t.uuid=(std::string)org::esb::util::PUUID();
        t.update();
        job.tasks().link(t);
      }
    }
    return result;
  }

  void JobHandler::handleGET(org::esb::core::Request*req, org::esb::core::Response*res) {
    org::esb::api::ServiceResponse*sres = ((org::esb::api::ServiceResponse*) res);
    JSONNode result(JSON_NODE);
    litesql::DataSource<db::Job> jobs = litesql::select<db::Job > (*getContext()->database);
    result.push_back(JSONNode("status", "ok"));
    JSONNode data(JSON_ARRAY);
    data.set_name("data");
    if (jobs.count() > 0) {

      foreach(db::Job steve, jobs.all()) {
        JSONNode job(JSON_NODE);
        job.push_back(JSONNode("uuid", steve.uuid.value()));
        JSONNode tasks(JSON_ARRAY);
        tasks.set_name("tasks");

        foreach(db::Task dbtask, steve.tasks().get().all()) {
          JSONNode task(JSON_NODE);
          task.push_back(JSONNode("uuid", dbtask.uuid.value()));
          task.push_back(JSONNode("name", dbtask.name.value()));
          task.push_back(JSONNode("parameter", dbtask.parameter.value()));
          task.push_back(JSONNode("status", dbtask.status.value()));
          task.push_back(JSONNode("statusmessage", dbtask.statustext.value()));
          task.push_back(JSONNode("progress", dbtask.progress.value()));
          tasks.push_back(task);
        }
        job.push_back(tasks);
        data.push_back(job);
      }
    }
    result.push_back(data);
    sres->setStatus(org::esb::api::ServiceResponse::OK);
    sres->getOutputStream()->write(result.write_formatted());
  }

  void JobHandler::handlePOST(org::esb::core::Request*req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest*sreq = ((org::esb::api::ServiceRequest*) req);
    org::esb::api::ServiceResponse*sres = ((org::esb::api::ServiceResponse*) res);
    JSONNode result(JSON_NODE);
    sres->setStatus(org::esb::api::ServiceResponse::BAD_REQUEST);
    //try {
    std::string postdata;
    sreq->getInputstream()->read(postdata);
    LOGDEBUG("POSTDATA = " << postdata);
    if (libjson::is_valid(postdata)) {
      LOGDEBUG("Data is valid");
      JSONNode inode = libjson::parse(postdata);
      std::string msg = checkJob(inode);
      if (msg.length() > 0) {
        JSONNode error(JSON_NODE);
        error.set_name("error");
        error.push_back(JSONNode("code", "attribute_error"));
        error.push_back(JSONNode("description", msg));
        result.push_back(error);

      } else {
        sres->setStatus(org::esb::api::ServiceResponse::OK);
        /*save method should here*/
        save(inode);
      }
    } else {
      JSONNode error(JSON_NODE);
      error.set_name("error");
      error.push_back(JSONNode("code", "parse_error"));
      error.push_back(JSONNode("description", "no valid json format given"));
      result.push_back(error);
    }
    /*} catch (std::exception &ex) {
      LOGDEBUG(ex.what());
      JSONNode error(JSON_NODE);
      error.set_name("error");
      error.push_back(JSONNode("code", "internal_error"));
      error.push_back(JSONNode("description", "internal error ouccured"));
      error.push_back(JSONNode("exception", ex.what()));
      result.push_back(error);
    }*/
    sres->getOutputStream()->write(result.write_formatted());

  }

  void JobHandler::save(JSONNode& node) {
  }

}
