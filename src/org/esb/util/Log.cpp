#include "Log.h"
#include "org/esb/config/config.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
using namespace boost::posix_time;
Log * Log::_logger=NULL;

Log::Log() {
  std::string logfile=org::esb::config::Config::getProperty("hive.base_path");
  logfile.append("/mhive.log");
  myfile.open(logfile.c_str(), std::ios::out | std::ios::app);
}

Log * Log::getLogger() {
  if(Log::_logger==NULL)
    Log::_logger=new Log();
  return Log::_logger;
}

void Log::log(std::stringstream & s, const char * l){
  myfile<<"["<< to_simple_string(microsec_clock::local_time())<< "] [ "<<l<< " ] " << s.str()<<"\r"<<std::endl;
}

void Log::debug(std::stringstream & s){
  log(s,"debug");
}

void Log::info(std::stringstream & s){
  log(s,"info");
}
void Log::warn(std::stringstream & s){
  log(s,"warn");
}
void Log::error(std::stringstream & s){
  log(s,"error");
}
void Log::fatal(std::stringstream & s){
  log(s,"fatal");
}


