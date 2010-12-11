/*----------------------------------------------------------------------
 *  File    : HiveService.cpp
 *  Author  : Jan Hölscher <jan.hoelscher@esblab.com>
 *  Purpose : Starter program for the hive
 *  Created : 22.02.2010, 12:30 by Jan Hölscher <jan.hoelscher@esblab.com>
 *
 *
 * MediaEncodingCluster, Copyright (C) 2001-2009   Jan Hölscher
 *
 * This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 * ----------------------------------------------------------------------
 */
#include <stdlib.h>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/lang/ProcessException.h"
#include <iostream>
#include <string>
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/Properties.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Process.h"
#include "org/esb/lang/ProcessListener.h"
#include <signal.h>

class PListener:public org::esb::lang::ProcessListener{
public:
  void onEvent(ProcessEvent&ev){
    LOGDEBUG("ProcessEvent received");
    if(ev.getEventType()==ProcessEvent::PROCESS_STOPPED){
      ::kill(getpid(),15);
    }
  }
};
//#include < linux/kernel.h >
//#include < linux/sched.h >
//#include < linux/module.h >
void open_webadmin(){
  std::list<std::string> arguments;
  std::string executable;
#ifdef __WIN32__
  executable="cmd.exe";
  //arguments.push_back("/c");
  arguments.push_back("start");
  arguments.push_back("http://localhost:8080");
#elif __APPLE__

  executable="/bin/sh";
  arguments.push_back("http://localhost:8080");
#else
  executable="/usr/bin/xdg-open";
  arguments.push_back("http://localhost:8080");
#endif
  org::esb::lang::Process(executable, arguments).start();
}

void open_dialog(std::string message, std::string type) {
  std::list<std::string> arguments;
  std::string executable;
#ifdef __WIN32__
  //std::list<std::string> arguments;
  std::string username=getenv("username");
  arguments.push_back(username);
  arguments.push_back(message);
  //Process p1("C:\\Windows\\system32\\msg.exe", args);
  executable="C:\\Windows\\system32\\msg.exe";
#elif __APPLE__
  //std::list<std::string> arguments;
  //arguments.push_back("/usr/bin/osascript");
  arguments.push_back("-e");
//  arguments.push_back("tell app \"Finder\" to display dialog \"test\"");
  arguments.push_back("tell app \"Finder\" to display dialog \""+message+"\" buttons{\"Ok\"} with icon 1");
  //org::esb::lang::Process("/usr/bin/osascript", arguments).start();
  executable="/usr/bin/osascript";
  //executable="/bin/sh";
#else
  //std::list<std::string> arguments;
  arguments.push_back("--"+type);
  arguments.push_back(message);
  executable="/usr/bin/kdialog";
//  org::esb::lang::Process("/usr/bin/kdialog", arguments).start();
#endif
  org::esb::lang::Process(executable, arguments).start();
}

int main(int argc, char**argv) {
  Log::open();
  /**
   * needing the base path from the executable
   * */
  int mode = 0;
  org::esb::io::File f(argv[0]);
  std::cout << f.getParent() << std::endl;
  std::string path = f.getParent();

  std::string executable = path;
  std::list<std::string> arguments;
  org::esb::util::Properties props;
  org::esb::io::File file(path.append("/../hive"));
  if (file.isDirectory()) {
    mode = 1;
  }
  org::esb::io::File logdir(path.append("/../log"));
  if (!logdir.exists()) {
    logdir.mkdir();
  }

#ifdef WIN32
  executable.append("/mhive.exe ");
  //replacing all slashes with backslashes
  int position = executable.find("/"); // find first slash
  while (position != string::npos) {
    executable.replace(position, 1, "\\");
    position = executable.find("/", position + 1);
  }
#else
  executable.append("/mhive");
#endif
  arguments.push_back("-a");
  //arguments.push_back("--debugmode");
  /*
  struct task_struct *task;
    for_each_process(task)
    {
    printk("%s [%d]\n",task->comm , task->pid);
    }*/

  org::esb::lang::Process pr(executable, arguments);
  PListener listener;
  pr.addProcessListener(&listener);
  pr.run();


  open_dialog("Media Encoding Cluster Server is running please open the webpage on http://localhost:8080 now","msgbox");
  arguments.clear();
  arguments.push_back("http://localhost:8080");
  org::esb::lang::Thread::sleep2(3500);
  open_webadmin();
  org::esb::lang::CtrlCHitWaiter::wait();
  open_dialog("Media Encoding Cluster\nStop signal received!\nhalting now","msgbox");
  try {
    pr.stop();
  } catch (org::esb::lang::ProcessException & ex) {
    try {
      //open_dialog("timeout received while halting Media Encoding Cluster \ntry to kill the process now\n","error");
      pr.kill();
    } catch (org::esb::lang::ProcessException & ex) {
      LOGERROR("Exception:" << ex.what());
    }
  }
  open_dialog("Media Encoding Cluster stopped!","msgbox");
}
