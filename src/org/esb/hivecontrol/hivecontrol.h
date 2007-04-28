#ifndef HIVECONTROL_H
#define HIVECONTROL_H
#include "org/esb/config/config.h"
#include "org/esb/net/ServerSocket.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/hivecontrol/ClientData.h"
#include "org/esb/hivecontrol/ClientHandler.h"

namespace org
{
  namespace esb
  {
    namespace hive
    {
      class HiveControl
      {
        public: 
          HiveControl();
          ~HiveControl();
	  void start();
	  void stop();
	private:
          void addClient();
          void removeClient();
	  bool _stop;
      }
      ;
    }
  }
}

#endif



