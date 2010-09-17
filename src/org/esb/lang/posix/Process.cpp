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
#include "org/esb/util/Log.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/lang/Thread.h"
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

        _processId = fork();
        char * args[2 + _arguments.size()];
        int a = 0;
        args[a++] = const_cast<char*> (_executable.c_str());
        std::list<std::string>::iterator arg_it = _arguments.begin();
        for (; arg_it != _arguments.end(); arg_it++) {
          args[a++] = const_cast<char*> ((*arg_it).c_str());
        }
        args[a++] = NULL;
        if (_processId < 0) {
          throw ProcessException("could not fork the process");
        }
        if (_processId == 0) {
          if (_executable.length() == 0)
            throw ProcessException("no executable given");
          int s = execv(_executable.c_str(), args);
          if (s != 0)
            throw ProcessException(std::string("could not start the process: ").append(_executable));
        } else {
          /**
           * wait for child process exit
           * */
          LOGDEBUG("pid=" << _processId);
          int status = 0;
          waitpid(_processId, &status, 0);
          LOGDEBUG("client exited:" << status);
        }
      }

      void Process::stop() {
        int result = ::kill(_processId, 15);
        if (result != 0)
          throw ProcessException(std::string("could not stop the process with pid: ").append(org::esb::util::StringUtil::toString(_processId)));
      }

      void Process::kill() {
        int result = ::kill(_processId, 9);
        if (result != 0)
          throw ProcessException(std::string("could not kill the process with pid: ").append(org::esb::util::StringUtil::toString(_processId)));
      }
    }
  }
}
