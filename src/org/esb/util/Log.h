#ifndef ORG_ESB_UTIL_LOGGER_H
#define ORG_ESB_UTIL_LOGGER_H
//#include "cxxtools/log.h"
//#include "cxxtools/loginit.h"
#include <iostream>

#define loginit(file)/*log_init(file)*/
#define logger(name)/*log_define(name)*/
#define logfatal(o1)std::cerr << o1<<std::endl;/*log_fatal(o1)*/
#define logerror(o1)std::cerr << o1<<std::endl;/*log_error(o1)*/
#define logwarn(o1)std::cout << o1<<std::endl;/*log_warn(o1)*/
#define loginfo(o1)std::cout << o1<<std::endl;/*log_info(o1)*/
#define logdebug(o1) std::cout << o1<<std::endl;/*log_debug(o1)*/


#endif


