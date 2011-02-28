/* 
 * File:   Project.cpp
 * Author: HoelscJ
 * 
 * Created on 6. Januar 2011, 17:29
 */

#include "Project.h"
#include "boost/property_tree/detail/rapidxml.hpp"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace hive {

      Project::Project(db::Project & project) {

      }

      Project::Project(std::string &xml) {
      }

      Project::~Project() {

      }

      void Project::setXml(std::string & xml) {
        rapidxml::xml_document<> _doc;
        try {
          _doc.parse < 0 > (const_cast<char*> (xml.c_str()));
        } catch (rapidxml::parse_error ex) {
          LOGERROR("Parser Exception :" << ex.what());
          LOGERROR("Parser Exception :" << ex.where<char>());
        }
      }
    }
  }
}
