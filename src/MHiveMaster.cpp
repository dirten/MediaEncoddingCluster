/* 
 * File:   MHiveMaster.cpp
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 16:11
 */

#include <cstdlib>
#include "org/esb/lang/Process.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  org::esb::io::File f(".");
   std::string exe=f.getPath()+"/mhivequeue";
#ifdef WIN32
  exe.append(".exe");
#endif
  LOGDEBUG("starting mhivequeue process");
  org::esb::lang::Process pQueue(exe);
  pQueue.run(true);


  exe=f.getPath()+"/mhivecore";
#ifdef WIN32
  exe.append(".exe");
#endif
  LOGDEBUG("starting mhivecore process");
  org::esb::lang::Process pCore(exe);
  pCore.run(true);

  exe=f.getPath()+"/mhivereader";
#ifdef WIN32
  exe.append(".exe");
#endif
  LOGDEBUG("starting mhivereader process");
  org::esb::lang::Process pReader(exe);
  pReader.run(true);

  exe=f.getPath()+"/mhivewriter";
#ifdef WIN32
  exe.append(".exe");
#endif
  LOGDEBUG("starting mhivewriter process");
  org::esb::lang::Process pWriter(exe);
  pWriter.run(true);

  LOGDEBUG("mhive started");
  org::esb::lang::CtrlCHitWaiter::wait();
  pQueue.stop();
  pCore.stop();
  pReader.stop();
  pWriter.stop();
  return 0;
}

