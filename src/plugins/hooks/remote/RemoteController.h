#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

#include <map>

#include "org/esb/lang/ProcessSupervisor.h"

namespace plugin{
  using org::esb::lang::ProcessSupervisor;
  class RemoteController
  {
    public:
      static void startPlugin(std::string & name,std::vector<std::string> & extra_args);
      static void stopPlugin(std::string & name);
      static bool running(std::string plugin_name);
    private:
      RemoteController();
      static void startProcessThread(std::string & name,std::string & command, std::vector<std::string> & args);
      static std::map<std::string,ProcessSupervisor*> _service_map;
      static std::map<std::string, std::vector<std::string> > _arguments;

  };
}
#endif // REMOTECONTROLLER_H
