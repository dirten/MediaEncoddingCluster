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
using namespace std;
using namespace org::esb::lang;
using namespace org::esb::io;
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
  boost::thread(boost::bind(&Process::start, &p));
  //p.start();
  LOGDEBUG("start returned");
  Thread::sleep2(10000);
  LOGDEBUG("stop child Process");
  p.stop();
  Thread::sleep2(5000);
  LOGDEBUG("kill child Process");
  try{
    p.kill();
    assert(false);
  }catch(org::esb::lang::ProcessException & ex){
    LOGERROR("Exception:"<<ex.what());
  }
  Thread::sleep2(5000);
  Log::close();
  return 0;
}

