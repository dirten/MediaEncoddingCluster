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
//#include < linux/kernel.h >
//#include < linux/sched.h >
//#include < linux/module.h >

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
  pr.run();
  org::esb::lang::CtrlCHitWaiter::wait();
  pr.stop();
  org::esb::lang::Thread::sleep2(5000);
  try {
    pr.kill();
  } catch (org::esb::lang::ProcessException & ex) {
    LOGERROR("Exception:" << ex.what());
  }
}
