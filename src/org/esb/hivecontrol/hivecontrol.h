#ifndef HIVECONTROL_H
#define HIVECONTROL_H
#include "org/esb/config/config.h"

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
          void addClient();
          void removeClient();
      }
      ;
    }
  }
}

#endif



