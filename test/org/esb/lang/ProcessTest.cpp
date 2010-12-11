/* 
 * File:   ProcessTest.cpp
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 14:10
 */

#include <cstdlib>
#include "org/esb/lang/Process.h"
#include "org/esb/lang/ProcessException.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "org/esb/io/File.h"
#include "org/esb/lang/ProcessListener.h"
using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;
int listener_count=0;
class PListener:public org::esb::lang::ProcessListener{
public:
  void onEvent(ProcessEvent&ev){
    //LOGDEBUG("ProcessEvent received");
    listener_count++;
  }
};

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  std::list<std::string> args;
  args.push_back("-a");
  args.push_back("-b");
  args.push_back("-c");
  args.push_back("-d a");
  File f(".");
  LOGDEBUG(f.getPath());
  std::string exe=f.getPath()+"/ProcessTestExecutable";
#ifdef WIN32
  exe.append(".exe");
#endif
  Process p(exe, args);
  PListener listener;
  p.addProcessListener(&listener);
  
  //boost::thread(boost::bind(&Process::start, &p));
  p.run(true);
  LOGDEBUG("start returned");
  Thread::sleep2(3000);
  LOGDEBUG("stop child Process");
  p.stop();
  Thread::sleep2(2000);
  LOGDEBUG("kill child Process");
  try{
    p.kill();
    assert(false);
  }catch(org::esb::lang::ProcessException & ex){
    LOGERROR("Exception:"<<ex.what());
  }
  Thread::sleep2(1000);
  //LOGDEBUG("testing Listener")
  assert(listener_count==2);
  Log::close();
  return 0;
}

