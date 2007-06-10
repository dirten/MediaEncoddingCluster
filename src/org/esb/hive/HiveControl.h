#ifndef ORG_ESB_HIVE_HIVECONTROL_H
#define ORG_ESB_HIVE_HIVECONTROL_H
#include "org/esb/util/Properties.h"
#include "org/esb/lang/Runnable.h"

using namespace org::esb::util;
using namespace org::esb::lang;
namespace org{
	namespace esb{
		namespace hive{
class HiveControl{
    private:
	HiveControl();
	bool isRunning;
	Properties * status;
	Runnable * listener;
    public:
	~HiveControl();
	bool startup(void);
	bool shutdown(void);
	Properties * getStatus();
	static HiveControl* getInstance();
};
		}
	}
}

#endif



