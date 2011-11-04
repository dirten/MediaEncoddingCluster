/* 
 * File:   Service.h
 * Author: HoelscJ
 *
 * Created on 10. Oktober 2011, 11:30
 */

#ifndef SERVICE_H
#define	SERVICE_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/hive/HiveClient.h"
#include "org/esb/hive/HiveClientAudio.h"

#include <list>
namespace clientcontroller {

  class Service : public org::esb::core::ServicePlugin , org::esb::signal::MessageListener {
  public:
    Service();
    virtual ~Service();
    void onMessage(org::esb::signal::Message &);
    void startService();
    void stopService();
    org::esb::core::OptionsDescription getOptionsDescription();
    org::esb::core::ServicePlugin::ServiceType getServiceType();
    void init();

  private:
    void startClientNodes(std::string host, int port);
    void stopClientNodes();
    std::list<Ptr<org::esb::hive::HiveClient> > _client_list;
    //Ptr<org::esb::hive::HiveClientAudio> _client_audio;
  };
  REGISTER_SERVICE("clientcontroller", Service);
}

#endif	/* SERVICE_H */

