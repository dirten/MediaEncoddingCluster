/* 
 * File:   JobController.h
 * Author: HoelscJ
 *
 * Created on 19. Juli 2010, 15:19
 */

#ifndef JOBCONTROLLER_H
#define	JOBCONTROLLER_H
#include "org/esb/db/hivedb.hpp"
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        class JobController {
        public:
          JobController();
          JobController(const JobController& orig);
          ~JobController();

          db::Job getJob();
          void shutdown();
        private:
          db::HiveDb _dbCon;
        };

      }
    }
  }
}

#endif	/* JOBCONTROLLER_H */

