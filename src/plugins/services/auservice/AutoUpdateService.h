#ifndef AUTOUPDATESERVICE_H
#define AUTOUPDATESERVICE_H
#include "org/esb/core/ServicePlugin.h"

class AutoUpdateService: public org::esb::core::ServicePlugin
{
public:
  AutoUpdateService();
  ~AutoUpdateService();
  void startService();
  void stopService();
  org::esb::core::OptionsDescription getOptionsDescription();
  //org::esb::core::ServicePlugin::ServiceType getServiceType();
  void init();

};
REGISTER_SERVICE("auservice", AutoUpdateService)

#endif // AUTOUPDATESERVICE_H
