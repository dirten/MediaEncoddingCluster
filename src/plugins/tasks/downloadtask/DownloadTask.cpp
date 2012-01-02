/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "DownloadTask.h"
#include "org/esb/core/TaskException.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/File.h"
#include "Poco/Exception.h"
#include "org/esb/lang/Ptr.h"

#include "org/esb/libjson/libjson.h"
namespace plugin {

  DownloadTask::DownloadTask():Task() {
  }

  DownloadTask::~DownloadTask() {
  }

  void DownloadTask::prepare() {
    Task::prepare();
    _srcuristr = getContext()->getEnvironment<std::string > ("downloadtask.src");
    _trguristr = getContext()->getEnvironment<std::string > ("downloadtask.trg");
    
    if (_trguristr.length() == 0) {
      _trguristr = getSink();
    }
    
    std::string data = getContext()->getEnvironment<std::string > ("data");
    if (libjson::is_valid(data)) {
      JSONNode node = libjson::parse(data);
      if (node.contains("infile")) {
        _srcuristr = node["infile"].as_string();
      }
    }
    if(_srcuristr.length()==0){
      throw org::esb::core::TaskException("No Source File given!");
    }
    /*
    if(_trguristr.length()==0){
      throw org::esb::core::TaskException("No Target File given!");
    }*/
  }

  org::esb::core::OptionsDescription DownloadTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("downloadtask");
    result.add_options()
            ("downloadtask.src", boost::program_options::value<std::string > ()->default_value(""), "Download task source")
            ("downloadtask.trg", boost::program_options::value<std::string > ()->default_value(""), "Download task target")
            ("data", boost::program_options::value<std::string > ()->default_value(""), "");
    return result;
  }

  int DownloadTask::getPadTypes() {
    return Task::SINK;
  }

  void DownloadTask::execute() {
    Task::execute();
    setProgressLength(1);
    Poco::File srcfile(_srcuristr);
    if (srcfile.exists()&&srcfile.isFile()) {
      Poco::File trgfile(_trguristr);
      srcfile.copyTo(_trguristr);
      setStatus(Task::DONE);
    } else {
      setStatus(Task::ERROR);
      throw org::esb::core::TaskException("Source File not found");
    }
    setProgress(1);
    LOGDEBUG("Download finish!");
  }

  typedef DownloadTask InputTask;
  REGISTER_TASK("InputTask", InputTask);
}

