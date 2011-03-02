/* 
 * File:   Job.cpp
 * Author: HoelscJ
 * 
 * Created on 1. März 2011, 15:30
 */
#include "org/esb/db/hivedb.hpp"
#include "JobCreator.h"
#include "boost/property_tree/detail/rapidxml.hpp"
#include "org/esb/util/Log.h"
#include "org/esb/hive/DatabaseService.h"
#include "org/esb/hive/FileImporter.h"
#include "org/esb/hive/JobUtil.h"
namespace org {
  namespace esb {
    namespace hive {
    using namespace rapidxml;
      JobCreator::JobCreator() {
      }

      JobCreator::~JobCreator() {
      }

      void JobCreator::setXml(std::string &data) {
        rapidxml::xml_document<> _doc;
        try {
          _doc.parse < 0 > (const_cast<char*> (data.c_str()));
        } catch (rapidxml::parse_error ex) {
          LOGERROR("Parser Exception :" << ex.what());
          LOGERROR("Parser Exception :" << ex.where<char>());
        }
        xml_node<>*job = _doc.first_node("job");
        if (!job) {
          LOGERROR("first element must be a job node");
          return;
        }

        xml_node<>*input = job->first_node("input");
        if (!input) {
          LOGERROR("/job/input not defined");
          return;
        }

        if (!input->first_attribute("url")) {
          LOGERROR("/job/input/@url not defined");
          return;
        }
        std::string input_url=input->first_attribute("url")->value();
        xml_node<>*preset = job->first_node("preset");
        if (!preset) {
          LOGERROR("/job/preset not defined");
          return;
        }

        if (!preset->first_attribute("name")) {
          LOGERROR("/job/preset/@name not defined");
          return;
        }
        std::string preset_name=preset->first_attribute("name")->value();
        
        xml_node<>*target = job->first_node("target");
        if (!target) {
          LOGERROR("/job/target not defined");
          return;
        }

        if (!target->first_attribute("url")) {
          LOGERROR("/job/target/@url not defined");
          return;
        }
        std::string target_url=target->first_attribute("url")->value();
        LOGDEBUG("importing input/url = "<< input_url);

        org::esb::hive::FileImporter imp;
        org::esb::io::File infile(input_url);
        db::MediaFile mfile=imp.import(infile);

        db::Preset pre(mfile.getDatabase());
        pre.name=preset_name;
        
        org::esb::hive::JobUtil::createJob(mfile, pre, target_url);
      }
    }
  }
}

