/* 
 * File:   Service.h
 * Author: HoelscJ
 *
 * Created on 7. Oktober 2011, 15:52
 */

#ifndef MQSERVICE_H
#define	MQSERVICE_H
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/lang/Ptr.h"
#include "org/esb/mq/QueueManager.h"


#include "exports.h"

namespace mqserver {

  class Service : public org::esb::core::ServicePlugin {
  public:
    Service();
    virtual ~Service();
    void MQSERVICE_EXPORT startService();
    void MQSERVICE_EXPORT stopService();
    org::esb::core::OptionsDescription MQSERVICE_EXPORT getOptionsDescription();
    org::esb::core::ServicePlugin::ServiceType getServiceType();
  private:
    void run();
    std::string _data_dir;
    Ptr<org::esb::mq::QueueManager>_qm;
  };
  REGISTER_SERVICE("mqserver", Service)
}

#endif	/* SERVICE_H */

