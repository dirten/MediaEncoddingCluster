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
#include "org/esb/util/Log.h"
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
        private:
          void start();
          void stop();
          bool _stop_signal;
          typedef org::esb::util::Queue<boost::shared_ptr<ProcessUnit>, 50 > ProcessUnitQueue;
          ProcessUnitQueue puQueue;
          org::esb::util::Queue<boost::shared_ptr<ProcessUnit>, 500 > audioQueue;

        };

      }
    }
  }
}

#endif	/* PROCESSUNITCONTROLLER_H */

