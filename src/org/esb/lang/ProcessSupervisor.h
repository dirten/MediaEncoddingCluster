#ifndef PROCESSSUPERVISOR_H
#define PROCESSSUPERVISOR_H
#include <string>
#include <vector>

#include "Poco/Process.h"

namespace org{
  namespace esb{
    namespace lang{

      class ProcessSupervisor
      {
        public:
          ProcessSupervisor(std::string & command, std::vector<std::string> & args, int restart_count);
          void kill();
          void start();
        private:
          void init();
          std::string _cmd;
          std::vector<std::string> _args;
          int _restart_count;
          bool _respawn;
          Poco::ProcessHandle * _handle;
      };
    }
  }
}

#endif // PROCESSSUPERVISOR_H
