/* 
 * File:   Process.h
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 13:45
 */

#ifndef PROCESS_H
#define	PROCESS_H
#include <string>
#include <list>
#include <inttypes.h>
#include "org/esb/lang/Thread.h"
#include <boost/thread/condition.hpp>
#include "org/esb/util/Log.h"

namespace org {
  namespace esb {
    namespace lang {

      class Process {
        classlogger("org.esb.lang.Process");
      public:
        Process(std::string e, std::list<std::string> args=std::list<std::string>(), std::string name=std::string());
        //Process(const Process& orig);
        virtual ~Process();
        void start();
        void run(bool restartable=false);
        void stop();
        void kill();
        bool isRunning();
      private:
        std::string _executable;
        std::string _name;
        std::list<std::string> _arguments;
        int32_t _processId;
        bool _running;
        bool _restartable;
        bool _stop;
        boost::condition process_shutdown_wait_condition;
        boost::mutex process_shutdown_wait_mutex;

        boost::condition process_started_wait_condition;
        boost::mutex process_started_wait_mutex;
      };
    }
  }
}

#endif	/* PROCESS_H */

