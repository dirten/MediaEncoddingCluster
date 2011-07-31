/* 
 * File:   MhiveClient.cpp
 * Author: HoelscJ
 *
 * Created on 6. September 2010, 12:50
 */

#include <cstdlib>
#include "org/esb/util/StringUtil.h"
using namespace std;
int server_pid=0;
int client_pid=0;
char *getmyline()
    {
            char *buf, *str = 0;
            buf = (char*)malloc(1); 
            int strlength = 0; 
            fread(buf, 1, 1, stdin); 
            while(!feof(stdin) && (*buf != '\n')) {
                    strlength++;
                    str = (char*)realloc(str, strlength);
                    *(str + strlength-1) = *buf; 
                    fread(buf, 1, 1, stdin);
            }
            if(feof(stdin))return NULL;
            free(buf);
            strlength++;
            str = (char*)realloc(str, strlength);
            *(str + strlength-1) = '\0';
            return str;
    }

bool startserver(){
  std::string executable = org::esb::config::Config::get("hive.base_path");
  executable.append("/bin");
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
  std::list<std::string> arguments;
  arguments.clear();
  //arguments.push_back(executable);
  //executable="/bin/sh";
  arguments.push_back("-r");
  arguments.push_back("--quite");
  //arguments.push_back("/dev/null");
  //arguments.push_back("--base="+bpath);
  //arguments.push_back("--debugmode");
  //LOGINFO("Starting mhive queue")
  org::esb::lang::Process *prq=new org::esb::lang::Process(executable, arguments);
//  PListener listener;
  //prq.addProcessListener(&listener);
  prq->run();
  server_pid=prq->getPid();
  return true;

}

bool stopserver(){
    org::esb::lang::Process proc(server_pid);
    try{
	proc.stop();
    }catch(org::esb::lang::ProcessException & ex){
	proc.kill();
    }
    server_pid=0;
    return true;
}

bool startclient(){
  std::string executable = org::esb::config::Config::get("hive.base_path");
  executable.append("/bin");
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
  std::list<std::string> arguments;
  arguments.clear();
  //arguments.push_back(executable);
  //executable="/bin/sh";
  arguments.push_back("-i");
  arguments.push_back("--quite");
  //arguments.push_back(">");
  //arguments.push_back("/dev/null");
  //arguments.push_back("--base="+bpath);
  //arguments.push_back("--debugmode");
  //LOGINFO("Starting mhive queue")
  org::esb::lang::Process *prq=new org::esb::lang::Process(executable, arguments);
//  PListener listener;
  //prq.addProcessListener(&listener);
  prq->run();
  client_pid=prq->getPid();
  return true;

}

bool stopclient(){
    org::esb::lang::Process proc(client_pid);
    try{
	proc.stop();
    }catch(org::esb::lang::ProcessException & ex){
	proc.kill();
    }
    return true;
}

/*
 * 
 */
int console() {
    char * line=NULL;
    std::cout<<"Welcome to the mhive console."<<std::endl;
    std::cout<<"type help to get a list of available commands."<<std::endl;
    do{
	std::cout <<"mhive>";
	if((line=getmyline())==NULL)break;
	std::string l=org::esb::util::StringUtil::trim(line,"\n");
	if(l=="help"){
	    std::cout<<"help                              -> print this help"<<std::endl;
	    std::cout<<"start server                      -> starts the mhive server"<<std::endl;
	    std::cout<<"start client                      -> starts the mhive client which will connect to first server found by auto discovery"<<std::endl;
	    std::cout<<"start client [IP|HOSTNAME][:PORT] -> starts the mhive client which will connect to the given server connection"<<std::endl;
	    std::cout<<"stop server                       -> stops the mhive server"<<std::endl;
	    std::cout<<"stop client                       -> stops the mhive client"<<std::endl;
	}else
	//if(l=="start"){
	//    std::cout<<"hee? start server, start client?"<<std::endl;
	//}else
	if(l=="start server"){
	    if(server_pid==0){
		startserver();
		std::cout << server_pid<<std::endl;
	    }else{
		std::cout << "server is allready running in this host!"<<std::endl;
	    }
	    //std::cout<<"start server"<<std::endl;
	}else
	if(l.find("start client")!=string::npos){
	    startclient();
	    std::cout<<"start client"<<std::endl;
	}else
	if(l=="stop server"){
	    stopserver();
	}else
	if(l=="stop client"){
	    stopclient();
	}else
	if(l=="exit"){
	    std::cout<<"exiting the mhive console, bye"<<std::endl;
	    break;
	}else
	if(l.length()>0){
	    std::cout<<"hee? unknown command "<<l<<std::endl;
	    std::cout<<"type help to get a list of available commands"<<std::endl;
	}
    }while(line);
  return 0;
}

