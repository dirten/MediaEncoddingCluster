/* 
 * File:   Service.h
 * Author: HoelscJ
 *
 * Created on 10. Oktober 2011, 11:30
 */

#ifndef JOBEXECUTOR_SERVICE_H
#define	JOBEXECUTOR_SERVICE_H
#include "org/esb/signal/MessageListener.h"
#include "org/esb/core/ServicePlugin.h"
#include "org/esb/core/Task.h"
#include "org/esb/core/Graph.h"

#include <list>
namespace jobexecutor {

  class Service : public org::esb::core::ServicePlugin , org::esb::signal::MessageListener {
    //logger("service.jobexecutor");
    classlogger("service.jobexecutor");
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
    void run();
    enum STATUS {
      NONE,
      RUNNING,
      STOPPING,
      STOPPED
    };
    STATUS _status;
    db::Job * _job;
    Ptr<org::esb::core::Task> _current_task;
    void actualizeProgress(org::esb::core::Graph *);
  };
  REGISTER_SERVICE("jobexecutor", Service);
}

#endif	/* SERVICE_H */

