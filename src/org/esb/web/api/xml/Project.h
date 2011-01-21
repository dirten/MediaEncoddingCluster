/* 
 * File:   Project.h
 * Author: HoelscJ
 *
 * Created on 7. Januar 2011, 13:14
 */

#ifndef PROJECT_H
#define	PROJECT_H
#include <string>
namespace org {
  namespace esb {
    namespace web {

      class Project {
      public:
        Project(std::string & xmldata);
        virtual ~Project();
      private:

      };
    }
  }
}

#endif	/* PROJECT_H */

