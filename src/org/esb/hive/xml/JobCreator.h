/* 
 * File:   Job.h
 * Author: HoelscJ
 *
 * Created on 1. März 2011, 15:30
 */

#ifndef JOB_H
#define	JOB_H
#include <string>
namespace org {
  namespace esb {
    namespace hive {

      class JobCreator {
      public:
        JobCreator();
        virtual ~JobCreator();
        void setXml(std::string &data);
      private:

      };
    }
  }
}

#endif	/* JOB_H */

