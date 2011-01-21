/* 
 * File:   Process.cpp
 * Author: HoelscJ
 * 
 * Created on 17. September 2010, 13:45
 */

#include "org/esb/lang/Process.h"
#include "org/esb/lang/ProcessException.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "org/esb/util/StringUtil.h"
#include "org/esb/io/File.h"

namespace org {
  namespace esb {
    namespace lang {

      std::list<Process*> Process::_process_list;

      Process::Process(std::string exe, std::list<std::string> args, std::string name) : _executable(exe), _arguments(args), _name(name) {
        _processId = 0;
        _running = false;
        _restartable = false;
        _stop = false;
        _process_list.push_back(this);
      }

      Process::~Process() {
        try {
          stop();
        } catch (ProcessException & ex) {
          LOGERROR(ex.what());
          try {
            kill();
          } catch (ProcessException& ex2) {
            LOGERROR(ex2.what());
          }
        }
      }

      void Process::start() {
        /*waiting 0,5 sec for the mutext condition*/
        org::esb::lang::Thread::sleep2(500);
        if (_executable.length() == 0) {
          LOGDEBUG("no executable given");
          throw ProcessException("no executable given");
        }
        //        std::cout << "starting process"<<std::endl;
        char * args[2 + _arguments.size()];
        int a = 0;
        //        args[a++] = "/bin/sh";
        //        args[a++] = "-c";
        //        std::string prg = "";//_executable;
        if (_name.length() == 0) {
          _name = _executable.c_str();
        }
        args[a++] = const_cast<char*> (_name.c_str());
        std::list<std::string>::iterator arg_it = _arguments.begin();
        LOGDEBUG("Using Executable: " << _executable);
        for (; arg_it != _arguments.end(); arg_it++) {
          LOGDEBUG("Arguments:" << (*arg_it));
          args[a++] = const_cast<char*> ((*arg_it).c_str());
          //          prg += " " + (*arg_it);
        }
        //        args[a++] = const_cast<char*> (prg.c_str());
        args[a++] = NULL;
        _processId = fork();
        if (_processId < 0) {
          LOGERROR("could not fork the process, EAGAIN");
          throw ProcessException("could not fork the process, EAGAIN");
        }
        if (_processId == 0) {
          /*child process here*/
          setsid();
          //          int s=system(args[2]);
          int s = execv(_executable.c_str(), args);
          if (s != 0) {
            notifyProcessListener(ProcessEvent(_processId, s, ProcessEvent::PROCESS_START_FAILED));
            LOGERROR("could not start the process: " << _executable);
            throw ProcessException(std::string("could not start the process: ").append(_executable));
          }
        } else {
          /*parent process here*/
          /**
           * wait for child process exit
           * */

          _running = true;

          LOGDEBUG("pid=" << _processId);
          process_started_wait_condition.notify_one();

          int status = 0;
          notifyProcessListener(ProcessEvent(_processId, status, ProcessEvent::PROCESS_STARTED));
          waitpid(_processId, &status, 0);
          if (!_stop && status != 0) {
            //            throw ProcessException("Process with pid ended unexepcted");
            LOGERROR("Process with pid " << _processId << " ended unexepcted -> " << _name);
          }
          LOGDEBUG("client Process with pid=" << _processId << " exited:" << status);
          notifyProcessListener(ProcessEvent(_processId, status, ProcessEvent::PROCESS_STOPPED));
          process_shutdown_wait_condition.notify_one();
          _running = false;
          if (_restartable) {
            run(_restartable);
          }
        }
      }

      void Process::run(bool restart, int count) {
        _restartable = restart;
        boost::mutex::scoped_lock process_started_lock(process_started_wait_mutex);
        boost::thread t(boost::bind(&Process::start, this));
        LOGDEBUG("waiting 5 sec. for process to start");
        if (process_started_wait_condition.timed_wait(process_started_lock, boost::posix_time::seconds(5))) {
          LOGDEBUG("process started");
        } else {
          LOGERROR("Process start timeout of 30 sec. reached");
          throw ProcessException("Process start timeout of 5 sec. reached");
        }
      }

      void Process::stop() {
        if (!_running)
          throw ProcessException(std::string("could not stop the process: ").append(_executable).append(" - process not running"));
        _stop = true;
        _restartable = false;
        int result = ::kill(_processId, 15);
        boost::mutex::scoped_lock process_shutdown_lock(process_shutdown_wait_mutex);
        if (!process_shutdown_wait_condition.timed_wait(process_shutdown_lock, boost::posix_time::seconds(30))) {
          //if (result != 0)
          throw ProcessException(std::string("could not stop the process with pid: ").append(org::esb::util::StringUtil::toString(_processId)));
        }
        _running = false;
      }

      void Process::kill() {
        if (!_running)
          throw ProcessException(std::string("could not kill the process: ").append(_executable).append(" - process not running"));
        _restartable = false;
        int result = ::kill(_processId, 9);
        if (result != 0)
          throw ProcessException(std::string("could not kill the process with pid: ").append(org::esb::util::StringUtil::toString(_processId)));
        _running = false;
        notifyProcessListener(ProcessEvent(_processId, -9, ProcessEvent::PROCESS_KILLED));
      }
      /*
      void Process::addProcessListener(ProcessListener listener){

      }

      void Process::notifyProcessListener(ProcessEvent&){

      }*/
    }
  }
}
