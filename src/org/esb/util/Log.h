#define DUMMY
#define FILELOGGING_
#ifndef ORG_ESB_UTIL_LOGGER_H
#define ORG_ESB_UTIL_LOGGER_H
//#include "cxxtools/log.h"
//#include "cxxtools/loginit.h"
//#include "boost/date_time/gregorian/gregorian.hpp"
//#include <boost/logging/format_fwd.hpp>
//#include "Datetime.h"
//#include <boost/thread.hpp>
#include <fstream>
#include <sstream>
#include <ostream>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>



#define loginit(file)/*log_init(file)*/
#define logger(name)/*log_define(name)*/

#define logfatal(o1)LOG4CPLUS_FATAL(log4cplus::Logger::getInstance("ERROR"),o1);//loglevel(o1, "fatal")/*log_fatal(o1)*/
#define LOGFATAL(o1,o2)LOG4CPLUS_FATAL(log4cplus::Logger::getInstance(o1),o2);//loglevel(o1, "fatal")/*log_fatal(o1)*/

#define logerror(o1)LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("ERROR"),o1);//loglevel(o1, "error")/*log_error(o1)*/
#define LOGERROR(o1,o2)LOG4CPLUS_ERROR(log4cplus::Logger::getInstance(o1),o2);//loglevel(o1, "error")/*log_error(o1)*/

#define logwarn(o1)LOG4CPLUS_WARN(Log::getLog4cplusLogger(),o1);//loglevel(o1, "warn")/*log_warn(o1)*/
#define LOGWARN(o1,o2)LOG4CPLUS_WARN(log4cplus::Logger::getInstance(o1),o2);//loglevel(o1, "warn")/*log_warn(o1)*/

#define loginfo(o1)LOG4CPLUS_INFO(Log::getLog4cplusLogger(),o1);//loglevel(o1, "info")/*log_info(o1)*/
#define LOGINFO(o1,o2)LOG4CPLUS_INFO(log4cplus::Logger::getInstance(o1),o2);//loglevel(o1, "warn")/*log_warn(o1)*/

#define logdebug(o1)LOG4CPLUS_DEBUG(Log::getLog4cplusLogger(),o1);//loglevel(o1, "debug")/*log_debug(o1)*/
#define LOGDEBUG(o1,o2)LOG4CPLUS_DEBUG(log4cplus::Logger::getInstance(o1),o2);//loglevel(o1, "warn")/*log_warn(o1)*/

#define logtrace(o1)LOG4CPLUS_TRACE(Log::getLog4cplusLogger(),o1);//loglevel(o1, "trace")/*log_debug(o1)*/
#define LOGTRACE(o1,o2)LOG4CPLUS_TRACE(log4cplus::Logger::getInstance(o1),o2);//loglevel(o1, "warn")/*log_warn(o1)*/

#define logtracemethod(o1)LOG4CPLUS_TRACE_METHOD(Log::getLog4cplusLogger(),o1);//loglevel(o1, "trace")/*log_debug(o1)*/
#define LOGTRACEMETHOD(o1,o2)LOG4CPLUS_TRACE_METHOD(log4cplus::Logger::getInstance(o1),o2);//loglevel(o1, "warn")/*log_warn(o1)*/
//#define logdebug(o1) { std::stringstream t; \
//t<< o1; \
//Log::getLogger()->debug(t);}

#include <iostream>

class Log{
public:
    static Log * getLogger();
    static log4cplus::Logger getLog4cplusLogger();
    static void open(std::string path);
    static void close();
    void debug(std::stringstream &);
    void info(std::stringstream &);
    void warn(std::stringstream &);
    void error(std::stringstream &);
    void fatal(std::stringstream &);
    void log(std::stringstream & s, const char * level);
	void log(std::string level, std::stringstream & s, const char * file, int line );
    ~Log();
private:
	static log4cplus::Logger logger;
	log4cplus::Logger trace_logger;
	log4cplus::Logger debug_logger;
	log4cplus::Logger info_logger;
	log4cplus::Logger warn_logger;
	log4cplus::Logger error_logger;
	log4cplus::Logger fatal_logger;
	log4cplus::Logger unknown_logger;
        log4cplus::ConfigureAndWatchThread * configureThread;

    Log();
    std::ofstream _myfile;
    static std::string _logpath;
    static Log * _logger;
};
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

#define loglevel(o1,level) {\
	myfile<< "[ "<<level<< "]" << o1<<"\r"<<std::endl; \
	 myfile.close();\
}
#else
#include <iostream>
#include <iomanip>

#define loglevel2(o1,level) {\
	std::cout<<std::dec<< "[ "<<level<< " ] "<<boost::this_thread::get_id()<<" ["<<__FILE__<<":"<<__LINE__<<"]" << o1<<"\r"<<std::endl; \
}
#define loglevel3(o1,level) {\
    std::stringstream t; \
    t <<boost::this_thread::get_id()<<" ["<<__FUNCTION__<<":"<<__LINE__<<"]" << o1; \
    Log::getLogger()->log(t, level);\
}
#define loglevel(o1,level) {\
    std::stringstream t; \
    t << o1; \
    Log::getLogger()->log(level, t ,__FUNCTION__,__LINE__);\
}
#endif
#endif
#endif


