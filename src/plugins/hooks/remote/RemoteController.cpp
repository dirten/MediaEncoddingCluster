#include "RemoteController.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hive/Environment.h"

namespace plugin{

  //using org::esb::lang::Thread;
  using org::esb::hive::Environment;

  std::map<std::string,ProcessSupervisor*> RemoteController::_service_map;


  void RemoteController::startProcessThread(std::string & name,std::string & command, std::vector<std::string> & args)
  {

    ProcessSupervisor *ps=new ProcessSupervisor(command, args, 5);
    _service_map[name]=ps;

    /*this call will start the Process and block until the process will be killed or is respwning to fast*/
    ps->start();

    /*after the process exited, delete the pointer*/
    delete ps;

    /*and remove it from the map*/
    _service_map.erase(name);
  }

  void RemoteController::startPlugin(std::string & name,std::vector<std::string> & extra_args)
  {
    if(!_service_map.count(name)>0){

      std::string cmd=Environment::get(Environment::EXE_PATH)+"/"+Environment::get(Environment::EXE_NAME);
      std::vector<std::string> args=extra_args;
      args.push_back("--explicit");
      args.push_back(name);
      args.push_back("--waitonstdin");
      go(RemoteController::startProcessThread, name, cmd, args);
    }
  }

  void RemoteController::stopPlugin(std::string & name)
  {
    if(_service_map.count(name)){
      _service_map[name]->kill();
    }
  }

  RemoteController::RemoteController()
  {
  }
}
