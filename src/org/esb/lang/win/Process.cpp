/* 
 * File:   Process.cpp
 * Author: HoelscJ
 * 
 * Created on 17. September 2010, 13:45
 */

#include "org/esb/lang/Process.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

namespace org {
  namespace esb {
    namespace lang {

      Process::Process(std::string exe, std::list<std::string> args) : _executable(exe), _arguments(args) {
        _processId = 0;
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
        char *vip = const_cast<char*> (_executable.append(" ").append(args).c_str());

        bool bWorked;
        STARTUPINFO suInfo;
        PROCESS_INFORMATION procInfo;

        memset(&suInfo, 0, sizeof (suInfo));
        memset(&procInfo, 0, sizeof (procInfo));
        suInfo.cb = sizeof (suInfo);

        bWorked = ::CreateProcess(m_Process.c_str(),
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
      }

      void Process::stop() {
        /*
         *
         * Stopping Application Services from configuration
         *
         */

        HANDLE hProcess;
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, _processId);
        TerminateProcess(hProcess, (DWORD) - 1);

      }

      void Process::kill() {
        /*
         *
         * Stopping Application Services from configuration
         *
         */

        HANDLE hProcess;
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, _processId);
        TerminateProcess(hProcess, (DWORD) - 1);

      }
    }
  }
}
