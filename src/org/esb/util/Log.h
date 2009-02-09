#define DUMMY
#ifndef ORG_ESB_UTIL_LOGGER_H
#define ORG_ESB_UTIL_LOGGER_H
//#include "cxxtools/log.h"
//#include "cxxtools/loginit.h"
//#include "boost/date_time/gregorian/gregorian.hpp"
//#include <boost/logging/format_fwd.hpp>
//#include "Datetime.h"

#include <fstream>

#define loginit(file)/*log_init(file)*/
#define logger(name)/*log_define(name)*/
#define logfatal(o1)loglevel(o1, "fatal")/*log_fatal(o1)*/
#define logerror(o1)loglevel(o1, "error")/*log_error(o1)*/
#define logwarn(o1)loglevel(o1, "warn")/*log_warn(o1)*/
#define loginfo(o1)loglevel(o1, "info")/*log_info(o1)*/
#define logdebug(o1)loglevel(o1, "debug")/*log_debug(o1)*/
#ifndef DUMMY 
#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>
using namespace boost::posix_time;
using namespace boost::gregorian;

#define loglevel(o1,level) {\
  ptime now = microsec_clock::local_time(); \
  date today = now.date(); \
  std::cout<<"["<<to_simple_string(now)<<"] ["<<level<<"]" << o1<<std::endl; \
}
#else
#ifdef FILELOGGING
#include <iostream>
#define loglevel(o1,level) {\
	std::ofstream myfile;\
	myfile.open ("data.log",std::ios::out | std::ios::app);\
	myfile<< "[ "<<level<< "]" << o1<<"\r"<<std::endl; \
	 myfile.close();\
}
#else
#include <iostream>
#define loglevel(o1,level) {\
	std::cerr<< "[ "<<level<< "]" << o1<<"\r"<<std::endl; \
}
#endif
#endif
#endif


