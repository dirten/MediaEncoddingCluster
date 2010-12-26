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
#include "ProcessListener.h"
namespace org {
  namespace esb {
    namespace lang {

      class Process {
        classlogger("org.esb.lang.Process");
      public:
        Process(int32_t pid):_processId(pid){
          _running = true;
          _restartable = false;
          _stop = false;
        }
        Process(std::string e, std::list<std::string> args=std::list<std::string>(), std::string name=std::string());
        //Process(const Process& orig);
        virtual ~Process();
        void start();
        void run(bool restartable=false, int restart_count=3);
        void stop();
        void kill();
        bool isRunning();
        int32_t getPid(){return _processId;}

        void addProcessListener(ProcessListener * l){
          _listener.push_back(l);
        }
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
        std::list<ProcessListener*> _listener;
        void notifyProcessListener(ProcessEvent ev){
          //LOGDEBUG("notify Listener:"<<_listener.size());
          std::list<ProcessListener*>::iterator it=_listener.begin();
          for(;it!=_listener.end();it++){
            //LOGDEBUG("notify!!!!")
            (*it)->onEvent(ev);
          }
        }
      };
    }
  }
}

#endif	/* PROCESS_H */

