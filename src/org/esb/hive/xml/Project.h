/* 
 * File:   Project.h
 * Author: HoelscJ
 *
 * Created on 6. Januar 2011, 17:29
 */

#ifndef PROJECT_H
#define	PROJECT_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/io/File.h"
#include <list>
namespace org {
  namespace esb {
    namespace hive {

      class Project {
      public:
        Project(db::Project & project);
        Project(std::string & xml);
        virtual ~Project();
        void setXml(std::string & xml);
      private:
        std::list<org::esb::io::File> _input_files;
      };
    }
  }
}

#endif	/* PROJECT_H */

