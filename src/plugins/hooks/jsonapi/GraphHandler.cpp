/* 
 * File:   JobHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 21. Oktober 2011, 14:18
 */

#include "GraphHandler.h"
#include "plugins/services/webservice/ServiceRequest.h"
#include "plugins/services/webservice/ServiceResponse.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/core/Task.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"

#include "org/esb/io/File.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/FileInputStream.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/lexical_cast.hpp"
#include "org/esb/util/UUID.h"
#include "org/esb/config/config.h"
#include "GraphVerifier.h"

#include "org/esb/core/Graph.h"
#include "org/esb/core/GraphParser.h"
#include "org/esb/core/GraphException.h"

namespace graphhandler {

  GraphHandler::GraphHandler() {

  }

  GraphHandler::~GraphHandler() {

  }

  void GraphHandler::handleRequest(org::esb::core::Request*req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest*sreq = ((org::esb::api::ServiceRequest*) req);
    LOGDEBUG("Url=" << sreq->getRequestURI());
    if (sreq->getRequestURI().find(_base_uri + "/graph") == 0) {
      if (sreq->getMethod() == "GET") {
        if (sreq->hasParameter("delete")) {
          handleDELETE(req, res);
        } else {
          handleGET(req, res);
        }
      } else if (sreq->getMethod() == "POST") {
        handlePOST(req, res);
      }
    }
  }

  void GraphHandler::init() {
    _base_uri = getContext()->getEnvironment<std::string > ("graphhandler.baseuri");
    LOGDEBUG("BaseUri GraphHandler=" << _base_uri);
  }

  org::esb::core::OptionsDescription GraphHandler::getOptionsDescription() {
    org::esb::core::OptionsDescription result("graphhandler");
    result.add_options()
            ("graphhandler.baseuri", boost::program_options::value<std::string > ()->default_value("/api/v1"), "base uri for the api requests");
    return result;
  }

  bool GraphHandler::contains(JSONNode& node, std::string name) {
    bool result = false;
    int size = node.size();
    if (size > 0) {
      for (int a = 0; a < size; a++) {
        JSONNode n = node[a];
        if (name == n.name()) {
          result = true;
        }
      }
    }
    return result;
  }

  void GraphHandler::handleDELETE(org::esb::core::Request*req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest*sreq = ((org::esb::api::ServiceRequest*) req);
    org::esb::api::ServiceResponse*sres = ((org::esb::api::ServiceResponse*) res);
    std::string user_path = org::esb::config::Config::get("hive.graph_path");
    if (sreq->hasParameter("uuid")) {
      std::string uuid = sreq->getParameter("uuid");
      org::esb::io::File f(user_path + "/" + uuid + ".graph");
      if (f.exists()) {
        f.deleteFile();
      }
    }
    sres->setStatus(org::esb::api::ServiceResponse::OK);
  }

  void GraphHandler::handleGET(org::esb::core::Request*req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest*sreq = ((org::esb::api::ServiceRequest*) req);
    org::esb::api::ServiceResponse*sres = ((org::esb::api::ServiceResponse*) res);
    JSONNode result(JSON_NODE);
    std::string user_path = org::esb::config::Config::get("hive.graph_path");
    if (sreq->hasParameter("submit") && sreq->hasParameter("uuid")) {
      performSubmit(req, res);
      return;
    } else
      if (sreq->hasParameter("uuid")) {
      std::string uuid = sreq->getParameter("uuid");
      org::esb::io::File f(user_path + "/" + uuid + ".graph");
      if (f.exists()) {
        org::esb::io::FileInputStream fis(&f);
        std::string ndata;
        fis.read(ndata);

        /**parsing json file*/
        if (libjson::is_valid(ndata)) {
          LOGDEBUG("Data is valid");
          JSONNode inode = libjson::parse(ndata);
          if (contains(inode, "uuid")) {
            uuid = inode["uuid"].as_string();
          } else {
            //f.deleteFile();
            //inode.push_back(JSONNode("uuid",uuid));
            //save(inode, uuid);
          }
          result.push_back(JSONNode("status", "ok"));
          inode.set_name("data");
          result.push_back(inode);
        }
      }
    } else {
      org::esb::io::File f(user_path + "/");
      org::esb::io::FileList files = f.listFiles();
      JSONNode data(JSON_ARRAY);
      data.set_name("data");
      result.push_back(JSONNode("status", "ok"));

      foreach(Ptr<org::esb::io::File> file, files) {
        /**load graph file*/
        org::esb::io::FileInputStream fis(file.get());
        std::string ndata;
        fis.read(ndata);
        std::string uuid = sreq->getUUID();

        /**parsing json file*/
        if (libjson::is_valid(ndata)) {
          LOGDEBUG("Data is valid");
          JSONNode inode = libjson::parse(ndata);
          std::string name;
          if (contains(inode, "name")) {
            name = inode["name"].as_string();
          }
          if (contains(inode, "uuid")) {
            uuid = inode["uuid"].as_string();
          } else {
            //file->deleteFile();
            //inode.push_back(JSONNode("uuid",uuid));
            //save(inode, uuid);
          }
          JSONNode file_node(JSON_NODE);
          file_node.set_name("graph");
          file_node.push_back(JSONNode("uuid", uuid));
          file_node.push_back(JSONNode("name", name));
          data.push_back(file_node);
        }
      }
      result.push_back(data);
    }

    sres->setStatus(org::esb::api::ServiceResponse::OK);
    sres->getOutputStream()->write(result.write_formatted());
  }

  void GraphHandler::handlePOST(org::esb::core::Request*req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest*sreq = ((org::esb::api::ServiceRequest*) req);
    org::esb::api::ServiceResponse*sres = ((org::esb::api::ServiceResponse*) res);
    JSONNode result(JSON_NODE);
    std::string postdata;
    sreq->getInputstream()->read(postdata, 1000000);
    LOGDEBUG("POSTDATA = " << postdata);
    if (libjson::is_valid(postdata)) {
      LOGDEBUG("Data is valid");
      JSONNode inode = libjson::parse(postdata);
      std::string uuid = sreq->getUUID();
      if (contains(inode, "uuid")) {
        uuid = inode["uuid"].as_string();
      } else {
        inode.push_back(JSONNode("uuid", uuid));
      }
      //std::string msg = checkGraph(inode);
      sres->setStatus(org::esb::api::ServiceResponse::OK);
      /*save method should here*/
      save(inode, uuid);
      result = inode;
    } else {
      JSONNode error(JSON_NODE);
      error.set_name("error");
      error.push_back(JSONNode("code", "parse_error"));
      error.push_back(JSONNode("description", "no valid json format given"));
      result.push_back(error);
    }
    sres->setStatus(org::esb::api::ServiceResponse::OK);
    sres->getOutputStream()->write(result.write_formatted());
  }

  void GraphHandler::save(JSONNode& node, std::string & uuid) {
    std::string user_path = org::esb::config::Config::get("hive.graph_path");
    org::esb::io::File f(user_path + "/" + uuid + ".graph");
    org::esb::io::FileOutputStream fos(&f);
    fos.write(node.write_formatted());
  }

  void GraphHandler::performSubmit(org::esb::core::Request*req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest*sreq = ((org::esb::api::ServiceRequest*) req);
    org::esb::api::ServiceResponse*sres = ((org::esb::api::ServiceResponse*) res);
    JSONNode result(JSON_NODE);
    std::string uuid = sreq->getParameter("uuid");
    boost::shared_ptr<db::HiveDb> db = getContext()->database;
    std::string user_path = org::esb::config::Config::get("hive.graph_path");
    org::esb::io::File f(user_path + "/" + uuid + ".graph");
    if (f.exists()) {
      org::esb::io::FileInputStream fis(&f);
      std::string ndata;
      fis.read(ndata);

      /**parsing json file*/
      try {
        org::esb::core::GraphParser graphparser(ndata);
        
      db::Job job(*getContext()->database);

      job.uuid = (std::string)org::esb::util::PUUID();
      job.status = db::Job::Status::Waiting;
      job.graph=ndata;
      job.created=0;
      job.update();

        //org::esb::core::Graph::createJob(list, getContext()->database);
        result.push_back(JSONNode("status", "ok"));
      } catch (org::esb::core::GraphException & ex) {
        result.push_back(JSONNode("status", "error"));
        result.push_back(JSONNode("message", ex.what()));
        result.push_back(JSONNode("element", ex.getElementId()));
      }
        /*
      if (libjson::is_valid(ndata)) {
        LOGDEBUG("Data is valid");
        JSONNode inode = libjson::parse(ndata);
        graph::GraphVerifier * v = graph::GraphVerifier::getInstance();
        if (!v->verify(inode, getContext()->database)) {
          result=v->getResult();
        } else {
          result.push_back(JSONNode("status", "ok"));
        }
      } else {
        result.push_back(JSONNode("status", "error"));
      }*/
    }
    LOGDEBUG("perform submit for uuid" << uuid);
    sres->setStatus(org::esb::api::ServiceResponse::OK);
    sres->getOutputStream()->write(result.write_formatted());
  }
}
