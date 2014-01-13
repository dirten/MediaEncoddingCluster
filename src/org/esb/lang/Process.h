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
//#include "org/esb/util/Log.h"
#include "ProcessListener.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace lang {

      class LANG_EXPORT Process {
        //classlogger("org.esb.lang.Process");
      public:
        Process(int32_t pid);
        Process(std::string e, std::list<std::string> args=std::list<std::string>(), std::string name=std::string());
        //Process(const Process& orig);
        virtual ~Process();
        void start();
        void run(bool restartable=false, int restart_count=3);
        void stop();
        void kill();
        void detouch();
        void addProcessListener(ProcessListener * l){_listener.push_back(l);}

        bool isRunning(){return _running;}
        int32_t getPid(){return _processId;}
        std::list<Process*> getProcessList(){return _process_list;}
        std::string getName(){return _name;}
        std::string getExecutable(){return _executable;}
        std::list<std::string> getArguments(){return _arguments;}
        bool isRestartable(){return _restartable;}
        
        static unsigned int getCpuCount();
      private:
        std::string _executable;
        std::string _name;
        std::list<std::string> _arguments;
        int32_t _processId;
        bool _running;
        bool _restartable;
        bool _stop;
        bool _detouched;
        boost::condition process_shutdown_wait_condition;
        boost::mutex process_shutdown_wait_mutex;

        boost::condition process_started_wait_condition;
        boost::mutex process_started_wait_mutex;
        std::list<ProcessListener*> _listener;
        void notifyProcessListener(ProcessEvent ev){
          std::list<ProcessListener*>::iterator it=_listener.begin();
          for(;it!=_listener.end();it++){
            (*it)->onEvent(ev);
          }
        }
        static std::list<Process*> _process_list;
      };
    }
  }
}

#endif	/* PROCESS_H */

