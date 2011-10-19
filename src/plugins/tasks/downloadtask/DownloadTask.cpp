/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "DownloadTask.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/FTPStreamFactory.h"
#include "Poco/URI.h"
#include "org/esb/core/PluginContext.h"

namespace plugin {

  DownloadTask::DownloadTask() {
    Poco::Net::HTTPStreamFactory::registerFactory();
    Poco::Net::FTPStreamFactory::registerFactory();
  }

  DownloadTask::~DownloadTask() {
    Poco::Net::HTTPStreamFactory::unregisterFactory();
    
  }
  
  void DownloadTask::prepare(){
    _srcuristr=getContext()->getEnvironment<std::string>("downloadtask.src");
    _trguristr=getContext()->getEnvironment<std::string>("downloadtask.trg");
  }
  
  void DownloadTask::execute(){
    Poco::URI uri(_uristr);
  }
}
