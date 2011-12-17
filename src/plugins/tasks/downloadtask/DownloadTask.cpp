/* 
 * File:   DownloadTask.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Oktober 2011, 11:41
 */

#include "org/esb/core/PluginContext.h"
#include "DownloadTask.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/File.h"
#include "Poco/Exception.h"
#include "org/esb/lang/Ptr.h"

#include "org/esb/libjson/libjson.h"
namespace plugin {

  DownloadTask::DownloadTask() {
  }

  DownloadTask::~DownloadTask() {
  }

  void DownloadTask::prepare() {
    _srcuristr = getContext()->getEnvironment<std::string > ("downloadtask.src");
    _trguristr = getContext()->getEnvironment<std::string > ("downloadtask.trg");
    if (_trguristr.length() == 0) {
      _trguristr = getSink();
    }
    std::string data = getContext()->getEnvironment<std::string > ("data");
    LOGDEBUG("data");
    if (libjson::is_valid(data)) {
      JSONNode node = libjson::parse(data);
      if (node.contains("infile")) {
        _srcuristr = node["infile"].as_string();
      }
    }
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
    LOGDEBUG("copy " << _srcuristr << " to " << _trguristr);
    //Poco::URI uri(_srcuristr);
    Poco::File srcfile(_srcuristr);
    if (srcfile.exists()) {
      Poco::File trgfile(_trguristr);
      srcfile.copyTo(_trguristr);
      setStatus(Task::DONE);
    } else {
      setStatus(Task::ERROR);
    }

    LOGDEBUG("Download finish!");
  }
  /*
    class RegisterDownloadTaskFactory : public org::esb::core::TaskFactory {
    public:

      RegisterDownloadTaskFactory() {
        org::esb::core::PluginRegistry::getInstance()->registerTaskFactory("DownloadTask", this);
      }

      ~RegisterDownloadTaskFactory() {
      }

      Ptr<org::esb::core::Task> create() {
        return Ptr<org::esb::core::Task > (new DownloadTask());
      }

      org::esb::core::OptionsDescription getOptionsDescription() {
        Ptr<org::esb::core::Task> t = Ptr<org::esb::core::Task>(new DownloadTask());
        return t->getOptionsDescription();
      }
    } RegisterDownloadTaskFactoryInstance;
   */
  //REGISTER_TASK("DownloadTask",DownloadTask );
  typedef DownloadTask InputTask;
  REGISTER_TASK("InputTask", InputTask);
}

