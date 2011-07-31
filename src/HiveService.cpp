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
#include "org/esb/grid/NodeResolver.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/lang/ProcessException.h"
#include <iostream>
#include <string>
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/Process.h"
#include "org/esb/lang/ProcessListener.h"
#include "org/esb/util/StringUtil.h"
#include <signal.h>


#include "org/esb/api/ApiWebServer.h"
#include "org/esb/api/ProcessHandler.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/config/config.h"

class PListener:public org::esb::lang::ProcessListener{
public:
  void onEvent(ProcessEvent&ev){
    LOGDEBUG("ProcessEvent received");
    if(ev.getEventType()==ProcessEvent::PROCESS_STOPPED){
#ifdef __WIN32__
        HANDLE hProcess;
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetCurrentProcessId());
        TerminateProcess(hProcess, (DWORD) - 1);
        CloseHandle(hProcess);
#else
        ::kill(getpid(), 15);
#endif
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

  executable="/usr/bin/open";
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
  std::string windir=getenv("windir");
  arguments.push_back(username);
  arguments.push_back(message);
  //Process p1("C:\\Windows\\system32\\msg.exe", args);

  executable=windir+"\\system32\\msg.exe";
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
  org::esb::io::File fbp(argv[0]);
  std::string base_path = org::esb::io::File(fbp.getParent()).getParent();

  org::esb::config::Config::setProperty("web.docroot",base_path);
  org::esb::api::ApiWebServer api_server(8888);
  org::esb::api::ProcessHandler proc_handler;
  api_server.addHandler(&proc_handler);
  org::esb::lang::CtrlCHitWaiter::wait();
  //return 0;

    /*first find out if there is running a server*/
  org::esb::grid::Node node;
  node.setData("type", "searcher");
  org::esb::grid::NodeResolver res(boost::asio::ip::address::from_string("0.0.0.0"), boost::asio::ip::address::from_string("239.255.0.1"), 6000, node);
  res.start();
  LOGDEBUG("Waiting 3 secs. to find all online nodes");
  org::esb::lang::Thread::sleep2(3000);
  org::esb::grid::NodeResolver::NodeList nodes = res.getNodes();
  org::esb::grid::NodeResolver::NodeList::iterator nodeit = nodes.begin();
  bool server_running = false;
  int client_count = 0;
  std::string host;
  int port=0;
  for (; nodeit != nodes.end(); nodeit++) {
    LOGDEBUG((*nodeit)->toString());
    LOGDEBUG("NodeType" << (*nodeit)->getData("type"));
    if ((*nodeit)->getData("type") == "server"){
      server_running = true;
      host = (*nodeit)->getIpAddress().to_string();
      port = atoi((*nodeit)->getData("port").c_str());
    }
    if ((*nodeit)->getData("type") == "client")
      client_count++;
  }
  res.stop();

  /**
   * needing the base path from the executable
   * */
  int mode = 0;
  org::esb::io::File f(argv[0]);
  std::cout << f.getParent() << std::endl;
  std::string path = f.getParent();
  std::string bpath = f.getParent();
  bpath.append("/..");
  std::string executable = path;
  std::list<std::string> arguments;
  
  org::esb::io::File file(path.append("/../hive"));
  if (!file.exists()) {
    file.mkdir();
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
  PListener listener;

  /*create internal database*/
  if(mode==1){
    arguments.clear();
    arguments.push_back("--install");
    arguments.push_back("--base="+bpath);
    LOGINFO("Create mhive database")
    org::esb::lang::Process prinst(executable, arguments, "mhiveinstaller");
    prinst.start();
  }
#ifdef USE_SAFMQ
  arguments.clear();
  arguments.push_back("-q");
  arguments.push_back("--base="+bpath);
  //arguments.push_back("--debugmode");
  LOGINFO("Starting mhive queue")
  org::esb::lang::Process prq(executable, arguments, "mhivequeue");
//  PListener listener;
  //prq.addProcessListener(&listener);
  prq.run();
#endif
  LOGINFO("Starting mhive webadmin")
  arguments.clear();
  arguments.push_back("-w");
  arguments.push_back("--base="+bpath);
  org::esb::lang::Process prw(executable, arguments,"mhiveweb");
  //prw.addProcessListener(&listener);
  prw.run();
  org::esb::lang::Process * pra=NULL;
  org::esb::lang::Process * prc=NULL;
  if(!server_running){
    LOGINFO("Starting mhive core service")
    arguments.clear();
    arguments.push_back("-r");
    arguments.push_back("--base="+bpath);
    //arguments.push_back("--debugmode");
    pra=new org::esb::lang::Process(executable, arguments,"mhivecore");
    //PListener listener;
    //pra.addProcessListener(&listener);
    pra->run();
    org::esb::lang::Thread::sleep2(2000);
    LOGINFO("Starting mhive client")
    LOGINFO("Client host="<<host<<" port="<<port);
    arguments.clear();
    arguments.push_back("-i");
//    arguments.push_back("-h 127.0.0.1");
    //arguments.push_back("-p "+org::esb::util::StringUtil::toString(port));
    //arguments.push_back("--base="+bpath);
    //arguments.push_back("--debugmode");
    prc=new org::esb::lang::Process(executable, arguments,"mhiveclient");
    //PListener listener;
    //pra.addProcessListener(&listener);
    prc->run();

  }else{
    LOGINFO("Starting mhive client")
    arguments.clear();
    arguments.push_back("-i");
//    arguments.push_back("-h "+host);
//    arguments.push_back("-p "+org::esb::util::StringUtil::toString(port));
    arguments.push_back("--base="+bpath);
    //arguments.push_back("--debugmode");
    prc=new org::esb::lang::Process(executable, arguments,"mhiveclient");
    //PListener listener;
    //pra.addProcessListener(&listener);
    prc->run();
  }



  open_dialog("Media Encoding Cluster Server is running please open the webpage on http://localhost:8080 now","msgbox");
  arguments.clear();
  arguments.push_back("http://localhost:8080");
//  org::esb::lang::Thread::sleep2(1000);
  open_webadmin();
  org::esb::lang::CtrlCHitWaiter::wait();
  open_dialog("Media Encoding Cluster\nStop signal received!\nhalting now","msgbox");
  try {
    pra->stop();
  } catch (org::esb::lang::ProcessException & ex) {
    try {
      pra->kill();
    } catch (org::esb::lang::ProcessException & ex) {
      LOGERROR("Exception:" << ex.what());
    }
  }
  delete pra;
  try {
    prc->stop();
  } catch (org::esb::lang::ProcessException & ex) {
    try {
      prc->kill();
    } catch (org::esb::lang::ProcessException & ex) {
      LOGERROR("Exception:" << ex.what());
    }
  }
  delete prc;

  try {
    prw.stop();
  } catch (org::esb::lang::ProcessException & ex) {
    try {
      prw.kill();
    } catch (org::esb::lang::ProcessException & ex) {
      LOGERROR("Exception:" << ex.what());
    }
  }
#ifdef USE_SAFMQ
  try {
    prq.stop();
  } catch (org::esb::lang::ProcessException & ex) {
    try {
      prq.kill();
    } catch (org::esb::lang::ProcessException & ex) {
      LOGERROR("Exception:" << ex.what());
    }
  }
#endif
  open_dialog("Media Encoding Cluster stopped!","msgbox");
}
