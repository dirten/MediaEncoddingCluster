/* 
 * File:   Project.h
 * Author: HoelscJ
 *
 * Created on 6. Januar 2011, 17:29
 */

#ifndef PROJECT_H
#define	PROJECT_H
namespace org {
  namespace esb {
    namespace hive {

      class Project {
      public:
        Project();
        virtual ~Project();
        void addInput(org::esb::io::File&);
      private:

      };
    }
  }
}

#endif	/* PROJECT_H */

