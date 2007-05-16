#ifndef HIVECONTROL_H
#define HIVECONTROL_H
#include "org/esb/util/Properties.h"
using namespace org::esb::util;
class HiveControl{
    private:
	HiveControl();
	bool isRunning;
	Properties * status;
    public:
	~HiveControl();
	bool startup(void);
	bool shutdown(void);
	Properties * getStatus();
	static HiveControl* getInstance();
};


#endif



