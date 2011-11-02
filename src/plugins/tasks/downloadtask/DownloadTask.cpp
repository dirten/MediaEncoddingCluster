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
#include "org/esb/lang/Ptr.h"
namespace plugin {

  DownloadTask::DownloadTask() {
  }

  DownloadTask::~DownloadTask() {
  }

  void DownloadTask::prepare() {
    _srcuristr = getContext()->getEnvironment<std::string > ("downloadtask.src");
    _trguristr = getContext()->getEnvironment<std::string > ("downloadtask.trg");
  }

  org::esb::core::OptionsDescription DownloadTask::getOptionsDescription() {
    org::esb::core::OptionsDescription result("downloadtask");
    result.add_options()
            ("downloadtask.src", boost::program_options::value<std::string > ()->default_value(""), "Download task source")
            ("downloadtask.trg", boost::program_options::value<std::string > ()->default_value(""), "Download task target");
    return result;
  }

  void DownloadTask::execute() {
    Poco::URI uri(_srcuristr);
    Ptr<std::istream> pStr = Poco::URIStreamOpener::defaultOpener().open(uri);
   
    std::ofstream osf(_trguristr.c_str(), std::ios::binary);
    Poco::StreamCopier::copyStream(*pStr.get(), osf);
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
  REGISTER_TASK("DownloadTask",DownloadTask );
}

