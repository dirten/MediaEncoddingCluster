/* 
 * File:   MhiveClient.cpp
 * Author: HoelscJ
 *
 * Created on 6. September 2010, 12:50
 */

#include <cstdlib>
#include "org/esb/util/StringUtil.h"
using namespace std;
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
  org::esb::hive::DatabaseService::start(org::esb::config::Config::getProperty("hive.base_path"));
  int port=atoi(org::esb::config::Config::getProperty("web.port"));
  new org::esb::api::JsonServer(port);
  org::esb::hive::ExportScanner expscan;
  org::esb::signal::Messenger::getInstance().addMessageListener(*new org::esb::hive::ExportScanner());
  org::esb::hive::HiveListener hive;
  org::esb::signal::Messenger::getInstance().addMessageListener(*new org::esb::hive::HiveListener());
  org::esb::hive::job::ProcessUnitController puw;
  org::esb::signal::Messenger::getInstance().addMessageListener(*new org::esb::hive::job::ProcessUnitController());

  org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("webserver", org::esb::hive::START));
  string base_path = org::esb::config::Config::getProperty("hive.base_path");
  org::esb::signal::Messenger::getInstance().sendMessage(org::esb::signal::Message().setProperty("processunitcontroller", org::esb::hive::START));
  org::esb::signal::Messenger::getInstance().sendMessage(org::esb::signal::Message().setProperty("hivelistener", org::esb::hive::START));
  org::esb::signal::Messenger::getInstance().sendRequest(org::esb::signal::Message().setProperty("exportscanner", org::esb::hive::START));

}

bool stopserver(){

}

bool startclient(){

}

bool stopclient(){

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
	    startserver();
	    std::cout<<"start server"<<std::endl;
	}else
	if(l.find("start client")!=string::npos){
	    std::cout<<"start client"<<std::endl;
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

