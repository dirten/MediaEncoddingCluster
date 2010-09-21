/* 
 * File:   Process.cpp
 * Author: HoelscJ
 * 
 * Created on 17. September 2010, 13:45
 */

#include "org/esb/lang/Process.h"
#include "org/esb/lang/ProcessException.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "org/esb/util/Log.h"
#include "boost/thread.hpp"
#include "boost/bind.hpp"
namespace org {
  namespace esb {
    namespace lang {

      Process::Process(std::string exe, std::list<std::string> args) : _executable(exe), _arguments(args) {
        _processId = 0;
        _running=false;
        _restartable=false;
      }

      Process::Process(const Process& orig) {
      }

      Process::~Process() {
      }

      void Process::start() {
        if (_executable.length() == 0)
          throw ProcessException("no executable given");

        std::string args;
        std::list<std::string>::iterator arg_it = _arguments.begin();
        for (; arg_it != _arguments.end(); arg_it++) {
          args.append((*arg_it)).append(" ");
        }
          //replacing all slashes with backslashes
        int position = _executable.find( "/" ); // find first slash
        while ( position != std::string::npos ) 
        {
          _executable.replace( position, 1, "\\" );
          position = _executable.find( "/", position + 1 );
        } 

        std::string a=(_executable+" "+args);
        char *vip = const_cast<char*>(a.c_str());

        BOOL bWorked;
        STARTUPINFO suInfo;
        PROCESS_INFORMATION procInfo;

        memset(&suInfo, 0, sizeof (suInfo));
        memset(&procInfo, 0, sizeof (procInfo));
        suInfo.cb = sizeof (suInfo);

        bWorked = ::CreateProcess(_executable.c_str(),
                vip, // can also be NULL
                NULL,
                NULL,
                FALSE,
                NORMAL_PRIORITY_CLASS,
                NULL,
                NULL,
                &suInfo,
                &procInfo);
        if (!bWorked) {
          throw ProcessException(std::string("could not start the process: ").append(_executable));
        }
        _processId = procInfo.dwProcessId;
        _running=true;
//        LOGDEBUG("Waiting for process");
        WaitForSingleObject( procInfo.hProcess, INFINITE );
        CloseHandle( procInfo.hProcess );
        LOGDEBUG("process ended:"<<_executable);
        _running=false;
        if(_restartable){
          LOGDEBUG("restarting!!!");
          run(_restartable);
        }
      }

      void Process::run(bool restartable) {
        _restartable=restartable;
        boost::thread(boost::bind(&Process::start, this));
      }

      void Process::stop() {
        /*
         *
         * Stopping Application Services from configuration
         *
         */
        if(!_running)
          throw ProcessException(std::string("could not stop the process: ").append(_executable).append(" - process not running"));
        _restartable=false;
        HANDLE hProcess;
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, _processId);
        TerminateProcess(hProcess, (DWORD) - 1);
        CloseHandle( hProcess );
        _running=false;
      }
      bool Process::isRunning() {
        return _running;
      }

      void Process::kill() {
        /*
         *
         * Stopping Application Services from configuration
         *
         */
        if(!_running)
          throw ProcessException(std::string("could not stop the process: ").append(_executable).append(" - process not running"));

        _restartable=false;
        HANDLE hProcess;
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, _processId);
        TerminateProcess(hProcess, (DWORD) - 1);
        _running=false;
      }
    }
  }
}
