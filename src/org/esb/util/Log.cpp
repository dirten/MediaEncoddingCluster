#include "Log.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include "LogConfigurator.cpp"
#include "org/esb/io/File.h"


using namespace boost::posix_time;

Log::Log() {
  std::string logconfig = "logging.properties";
  configureThread = NULL;

  //    configureThread = new log4cplus::ConfigureAndWatchThread(LOG4CPLUS_TEXT(logconfig), 5 * 1000);
  /*
    log4cplus::helpers::Properties props;
  //  props.setProperty(LOG4CPLUS_TEXT(""),LOG4CPLUS_TEXT(""));
    props.setProperty(LOG4CPLUS_TEXT("rootLogger"),LOG4CPLUS_TEXT("DEBUG, STDOUT"));
    props.setProperty(LOG4CPLUS_TEXT("appender.STDOUT"),LOG4CPLUS_TEXT("log4cplus::ConsoleAppender"));

    log4cplus::PropertyConfigurator config(props, log4cplus::Logger::getDefaultHierarchy());
   */
  //  org::esb::util::LogConfigurator config;
  log4cplus::PropertyConfigurator config(LOG4CPLUS_TEXT("logging.properties"));

  config.configure();

  trace_logger = log4cplus::Logger::getInstance("trace");
  debug_logger = log4cplus::Logger::getInstance("DEBUG");
  info_logger = log4cplus::Logger::getInstance("info");
  warn_logger = log4cplus::Logger::getInstance("warn");
  error_logger = log4cplus::Logger::getInstance("error");
  fatal_logger = log4cplus::Logger::getInstance("fatal");
  unknown_logger = log4cplus::Logger::getInstance("unknown");
  /*
  if(Log::_logpath.length()>0){
      std::string logfile=Log::_logpath;//org::esb::config::Config::getProperty("hive.base_path");
      logfile.append("/mhive.log");
      _myfile.open(logfile.c_str(), std::ios::out | std::ios::app);
      std::cout.rdbuf(_myfile.rdbuf());
      std::cerr.rdbuf(_myfile.rdbuf());
    }*/
}

Log::~Log() {
  _myfile.close();
  delete configureThread;
}
void Log::open(std::string path) {
  std::cout << "enable logging to "<<path << std::endl;
  if (path.length() == 0) {
    log4cplus::BasicConfigurator::doConfigure();
  } else {
    log4cplus::PropertyConfigurator config(LOG4CPLUS_TEXT(path+"/logging.properties"));
    log4cplus::helpers::Properties & props = const_cast<log4cplus::helpers::Properties&> (config.getProperties());
    props.setProperty(LOG4CPLUS_TEXT("appender.MAIN.File"), LOG4CPLUS_TEXT(path+"/mhive-debug.log"));
    props.setProperty(LOG4CPLUS_TEXT("appender.ERROR.File"), LOG4CPLUS_TEXT(path+"/mhive-error.log"));
    config.configure();
  }
}

void Log::close() {
  delete getLogger();
}

Log * Log::getLogger(std::string logger) {
  return NULL;
}


void Log::log(std::string l, std::stringstream & s, const char * file, int line) {
  if (l == "trace") {
    LOG4CPLUS_TRACE_METHOD(trace_logger, s.str());
  } else if (l == "debug") {
    LOG4CPLUS_DEBUG(debug_logger, s.str());
  } else if (l == "info") {
    LOG4CPLUS_INFO(info_logger, s.str());
  } else if (l == "warn") {
    LOG4CPLUS_WARN(warn_logger, s.str());
  } else if (l == "error") {
    LOG4CPLUS_ERROR(error_logger, s.str());
  } else if (l == "fatal") {
    LOG4CPLUS_FATAL(fatal_logger, s.str());
  } else {
    LOG4CPLUS_DEBUG(unknown_logger, s.str());
  }
  //	std::cout<<"["<< to_simple_string(microsec_clock::local_time())<< "]["<<l<< "]["<<file<<":"<<line<<"] " << s.str()<<"\r"<<std::endl;
}

void Log::log(std::stringstream & s, const char * l) {
  std::cout << "[" << to_simple_string(microsec_clock::local_time()) << "] [ " << l << " ] " << s.str() << "\r" << std::endl;
}

void Log::debug(std::stringstream & s) {
  log(s, "debug");
}

void Log::info(std::stringstream & s) {
  log(s, "info");
}

void Log::warn(std::stringstream & s) {
  log(s, "warn");
}

void Log::error(std::stringstream & s) {
  log(s, "error");
}

void Log::fatal(std::stringstream & s) {
  log(s, "fatal");
}


