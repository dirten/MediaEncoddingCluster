/* 
 * File:   ConfigHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 17. Oktober 2011, 10:46
 */

#include "ConfigHandler.h"
#include "plugins/services/webservice/ServiceRequest.h"
#include "plugins/services/webservice/ServiceResponse.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/util/StringUtil.h"
#include "boost/program_options/options_description.hpp"
#include "org/esb/libjson/libjson.h"
namespace api {

  ConfigHandler::ConfigHandler() {
  }

  ConfigHandler::~ConfigHandler() {
  }

  void ConfigHandler::init() {
    _base = getContext()->getEnvironment<std::string > ("configapi.baseuri");
    LOGDEBUG("Base : " << _base);
    LOGDEBUG("Base length: " << _base.length());
    _uri_len = static_cast<int> (_base.length()) + 8;
  }

  void ConfigHandler::handleRequest(org::esb::core::Request * req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest * sreq = static_cast<org::esb::api::ServiceRequest *> (req);
    org::esb::api::ServiceResponse * sres = static_cast<org::esb::api::ServiceResponse *> (res);
    if (sreq->getRequestURI().find(_base + "/config") == 0) {
      if (sreq->getMethod() == "GET") {
        handleListConfig(req, res);
      } else if (sreq->getMethod() == "POST") {
        handleSetConfig(req, res);
      }
    }
  }

  void ConfigHandler::handleListConfig(org::esb::core::Request * req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest * sreq = static_cast<org::esb::api::ServiceRequest *> (req);
    org::esb::api::ServiceResponse * sres = static_cast<org::esb::api::ServiceResponse *> (res);
    LOGDEBUG("Uri Length : " << _uri_len);
    LOGDEBUG("Uri : " << sreq->getRequestURI());
    JSONNode result(JSON_ARRAY);
    if (sreq->getRequestURI().length() < _uri_len) {
      result = JSONNode(JSON_NODE);
      result.set_name("error");
      result.push_back(JSONNode("status", "error"));
      result.push_back(JSONNode("desc", "url not parsable"));
      sres->setStatus(org::esb::api::ServiceResponse::NOT_FOUND);
    } else {
      std::string key = sreq->getRequestURI().substr(_uri_len);
      LOGDEBUG("config key:" << key);

      result.set_name("data");
      if (key.length() == 0) {
        std::list<std::string> keys = org::esb::core::PluginRegistry::getInstance()->getPluginNameList();

        foreach(std::string key, keys) {
          JSONNode data(JSON_NODE);
          data.push_back(JSONNode("plugin", key));
          JSONNode data_opt(JSON_ARRAY);
          data_opt.set_name("options");
          handleKey(key, data_opt);
          data.push_back(data_opt);
          result.push_back(data);
        }
      } else {
        handleKey(key, result);
      }
      sres->setStatus(org::esb::api::ServiceResponse::OK);
    }
    sres->getOutputStream()->write(result.write_formatted());
  }

  void ConfigHandler::handleKey(std::string key, JSONNode & result) {
    org::esb::core::OptionsDescription opt = org::esb::core::PluginRegistry::getInstance()->getOptionsDescription(key);
    typedef boost::shared_ptr<boost::program_options::option_description> option;

    foreach(const option & o, opt.options()) {
      boost::any data;
      o->semantic()->apply_default(data);
      std::string def;
      if (data.type() == typeid (int)) {
        def = org::esb::util::StringUtil::toString(boost::any_cast<int>(data));
      } else if (data.type() == typeid (double)) {
        def = org::esb::util::StringUtil::toString(boost::any_cast<double>(data));
      } else if (data.type() == typeid (bool)) {
        def = org::esb::util::StringUtil::toString(boost::any_cast<bool>(data));
      } else {
        def = org::esb::util::StringUtil::toString(boost::any_cast<std::string > (data));
      }

      JSONNode row(JSON_NODE);
      row.push_back(JSONNode("name", o->long_name()));
      row.push_back(JSONNode("value", org::esb::core::PluginRegistry::getInstance()->getConfigData(o->long_name())));
      row.push_back(JSONNode("default", def));
      row.push_back(JSONNode("plugin", key));
      result.push_back(row);
    }
  }

  void ConfigHandler::handleSetConfig(org::esb::core::Request * req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest * sreq = static_cast<org::esb::api::ServiceRequest *> (req);
    org::esb::api::ServiceResponse * sres = static_cast<org::esb::api::ServiceResponse *> (res);
    try {
      std::string postdata;
      sreq->getInputstream()->read(postdata);
      if (libjson::is_valid(postdata)) {
        JSONNode inode = libjson::parse(postdata);
        if (contains(inode, "key")) {
          if (contains(inode, "val")) {
            litesql::DataSource<db::Config> confs = litesql::select<db::Config > (*getContext()->database, db::Config::Configkey == inode["key"].as_string());
            if(confs.count()>0){
              db::Config config=confs.one();
              config.configval=inode["val"].as_string();
              config.update();
            }else{
              db::Config config(*getContext()->database);
              config.configkey=inode["key"].as_string();
              config.configval=inode["val"].as_string();
              config.update();
            }
            sres->setStatus(org::esb::api::ServiceResponse::OK);

          }
        }
      }
    } catch (std::exception & ex) {

    }

  }

  org::esb::core::OptionsDescription ConfigHandler::getOptionsDescription() {
    org::esb::core::OptionsDescription result("configapi");
    result.add_options()
            ("configapi.baseuri", boost::program_options::value<std::string > ()->default_value("/api/v1"), "base uri for the config api requests");
    return result;
  }

  bool ConfigHandler::contains(JSONNode& node, std::string name) {
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

}

