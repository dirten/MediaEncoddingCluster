#include "ProcessSupervisor.h"
#include <signal.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Poco/Pipe.h"

//#include "org/esb/hive/Environment.h"


namespace org{
  namespace esb{
    namespace lang{

      using boost::posix_time::ptime;
      using boost::posix_time::time_duration;
      using boost::posix_time::microsec_clock;

      ProcessSupervisor::ProcessSupervisor(std::string & command, std::vector<std::string> & args, int restart_count)
      :_cmd(command),
      _args(args),
      _restart_count(restart_count),
      _respawn(true)
      {

      }
      void ProcessSupervisor::kill()
      {
        _respawn=false;
        Poco::Process::kill(_handle->id());
      }

      void ProcessSupervisor::start()
      {
        init();
      }

      void ProcessSupervisor::init()
      {
        int fast_respawn_count=_restart_count;
        while(_respawn){

          ptime start = microsec_clock::local_time();

          Poco::Pipe outPipe;
          Poco::Pipe inPipe;
          Poco::Pipe errPipe;

          _handle=new Poco::ProcessHandle(Poco::Process::launch(_cmd, _args,&inPipe, &outPipe, &errPipe));
          _handle->wait();

          ptime end = microsec_clock::local_time();
          time_duration td=end - start;
          int64_t respawn_after=td.total_milliseconds();

          if(respawn_after<1000){
            fast_respawn_count--;
          }else{
            fast_respawn_count=_restart_count;
          }

          if(fast_respawn_count==0){
            _respawn=false;
            std::cout << "child process respawning to fast, exiting"<<std::endl;
          }
        }
      }
    }
  }
}
