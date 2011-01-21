/* 
 * File:   Project.cpp
 * Author: HoelscJ
 * 
 * Created on 7. Januar 2011, 13:14
 */

#include "Project.h"
#include "boost/property_tree/detail/rapidxml.hpp"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace web {

      Project::Project(std::string &xmldata) {
        rapidxml::xml_document<> _doc;
        try {
          _doc.parse < 0 > (const_cast<char*> (xmldata.c_str()));
        } catch (rapidxml::parse_error ex) {
          LOGERROR("Parser Exception :" << ex.what());
          LOGERROR("Parser Exception :" << ex.where<char>());
          return;
        }
      }

      Project::~Project() {
      }
    }
  }
}
