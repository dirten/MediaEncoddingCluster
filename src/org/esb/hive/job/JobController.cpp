/* 
 * File:   JobController.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Juli 2010, 15:19
 */

#include "JobController.h"
#include "org/esb/config/config.h"
          using namespace org::esb::config;
namespace org {
  namespace esb {
    namespace hive {
      namespace job {
        
        JobController::JobController():_dbCon("mysql",Config::getProperty("db.url")) {
          LOGTRACEMETHOD("JobController::JobController():_dbCon(\"mysql\",Config::getProperty(\"db.url\"))")
          
        }

        JobController::JobController(const JobController& orig):_dbCon(orig._dbCon) {
          LOGTRACEMETHOD("JobController::JobController(const JobController& orig):_dbCon(orig._dbCon)")
        }

        JobController::~JobController() {
          LOGTRACEMETHOD("JobController::~JobController()")
        }
        void JobController::shutdown() {
          
        }
        db::Job JobController::getJob() {
          db::Job job=litesql::select<db::Job>(_dbCon,db::Job::Begintime<=1).one();
          
          return job;
        }
      }
    }
  }
}

