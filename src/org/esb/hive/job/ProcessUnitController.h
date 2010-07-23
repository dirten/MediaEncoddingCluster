/* 
 * File:   ProcessUnitController.h
 * Author: HoelscJ
 *
 * Created on 16. Juli 2010, 17:09
 */

#ifndef PROCESSUNITCONTROLLER_H
#define	PROCESSUNITCONTROLLER_H
#include "org/esb/db/hivedb.hpp"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"
#include "org/esb/util/Queue.h"
//#include "org/esb/util/Log.h"
#include "org/esb/hive/job/ProcessUnit.h"
namespace org {
  namespace esb {
    namespace hive {
      namespace job {

        class ProcessUnitController : public org::esb::signal::MessageListener {
          classlogger("org.esb.hive.job.ProcessUnitController");
        public:
          /*
          enum COMMAND {
            GET_PROCESS_UNIT=,
            GET_AUDIO_PROCESS_UNIT,
            PUT_PROCESS_UNIT,
            PUT_AUDIO_PROCESS_UNIT
          };*/

          ProcessUnitController();
          ProcessUnitController(const ProcessUnitController& orig);
          virtual ~ProcessUnitController();
          void onMessage(org::esb::signal::Message & msg);
          void processJob(db::Job job);
          boost::shared_ptr<ProcessUnit> getProcessUnit();
          boost::shared_ptr<ProcessUnit> getAudioProcessUnit();
          bool putProcessUnit(boost::shared_ptr<ProcessUnit> & unit);
        private:
          void start();
          void stop();
          bool _stop_signal;
          typedef org::esb::util::Queue<boost::shared_ptr<ProcessUnit>, 50 > ProcessUnitQueue;
          ProcessUnitQueue puQueue;
          org::esb::util::Queue<boost::shared_ptr<ProcessUnit>, 500 > audioQueue;
          boost::mutex queue_empty_wait_mutex;
          boost::condition queue_empty_wait_condition;
          boost::mutex put_pu_mutex;
          boost::mutex get_pu_mutex;
          boost::mutex db_con_mutex;

          bool _isStopSignal;
          bool _isRunning;

          db::HiveDb _dbCon;
          db::HiveDb _dbJobCon;

//          boost::shared_ptr<db::Job> _actual_job;
        };

      }
    }
  }
}

#endif	/* PROCESSUNITCONTROLLER_H */

