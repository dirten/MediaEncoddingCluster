#ifndef ORG_ESB_UTIL_SCOPEDTIMECOUNTER_H
#define ORG_ESB_UTIL_SCOPEDTIMECOUNTER_H
#include <string>
//#include "boost/date_time/posix_time/posix_time.hpp"
//using namespace boost::posix_time;

class ScopedTimeCounter
{
public:
	ScopedTimeCounter(std::string name);
	~ScopedTimeCounter(void);
private:
//	ptime _start;
//	ptime _end;
//	std::string _name;
};
#endif

