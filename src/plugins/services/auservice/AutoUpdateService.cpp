#include "AutoUpdateService.h"
#include "org/esb/core/PluginContext.h"
#include "org/esb/signal/Messenger.h"
#include "org/esb/util/StringUtil.h"

#include "Poco/URI.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/NetException.h"
#include "Poco/StreamCopier.h"
namespace auservice {

  using org::esb::util::StringUtil;

  AutoUpdateService::AutoUpdateService()
  {
    _host="localhost";
    _port="8080";
    _stop=false;
  }

  AutoUpdateService::~AutoUpdateService()
  {

  }
  void AutoUpdateService::onMessage(org::esb::signal::Message & msg)
  {
    if (msg.containsProperty("server_up_event")) {
      LOGDEBUG("Server Up event received");
      _host = msg.getProperty<std::string > ("host");
      _port = msg.getProperty<std::string>("webport");
      _stop=false;
      go(AutoUpdateService::run, this);
    } else if (msg.containsProperty("server_down_event")) {
      LOGDEBUG("Server Down event received");
      _stop=true;
    }
  }

  void AutoUpdateService::startService()
  {
    if (getContext()->getEnvironment<bool>("auservice.autodiscovery")) {
      org::esb::signal::Messenger::getInstance().addMessageListener(*this);
    } else {
      _host = getContext()->getEnvironment<std::string > ("auservice.host");
      _port = getContext()->getEnvironment<std::string>("auservice.port");
    }
  }

  void AutoUpdateService::stopService()
  {
    _stop=true;
    org::esb::signal::Messenger::getInstance().removeMessageListener(*this);
  }

  org::esb::core::OptionsDescription AutoUpdateService::getOptionsDescription()
  {
    org::esb::core::OptionsDescription result("auservice");
    result.add_options()
    ("auservice.autodiscovery", boost::program_options::value<bool >()->default_value(true), "use autodiscover to connect the client to the updateserver");
    return result;
  }
  /*
org::esb::core::ServicePlugin::ServiceType AutoUpdateService::getServiceType(){

}
*/
  void AutoUpdateService::init()
  {

  }

  void AutoUpdateService::run()
  {

#if __LINUX__
      std::string system="Linux";
#elif __APPLE__
      std::string system="Darwin";
#elif __WIN32__
      std::string system="win32";
#endif

    while(!_stop){
      LOGDEBUG("check for updates on server:"<<_host<<" with higher version of : "<< MHIVE_VERSION<< " for system "<<system)
      Poco::Net::HTTPStreamFactory fac;
      std::string uri_str="http://"+_host+":"+_port+"/api/v1/update";
      Poco::URI uri(uri_str);
      LOGDEBUG("URI:"<<uri.toString());
      std::istream *istr=fac.open(uri);
      std::string data;
      Poco::StreamCopier::copyToString64(*istr, data);
      LOGDEBUG("data received:"<<data)
      if(data.compare(MHIVE_VERSION)>0){
        std::string filename="MediaEncodingCluster-"+data+"-"+system+".tgz";
        uri_str+="/"+filename;
        Poco::URI uri(uri_str);
        LOGDEBUG("URI:"<<uri.toString());
        try{
          std::istream *istr=fac.open(uri);
          std::string data;
          Poco::StreamCopier::copyToString64(*istr, data);
        }catch(Poco::Net::HTTPException& ex){
          LOGERROR("failed download update: "<<ex.displayText())
        }
      }

      /*once an hour*/
      Thread::sleep2(1000*60*60);
    }
  }
}
