#include "Log.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
using namespace boost::posix_time;
Log * Log::_logger=NULL;
std::string Log::_logpath="";
Log::Log() {
  if(Log::_logpath.length()>0){
    std::string logfile=Log::_logpath;//org::esb::config::Config::getProperty("hive.base_path");
    logfile.append("/mhive.log");
    _myfile.open(logfile.c_str(), std::ios::out | std::ios::app);
    std::cout.rdbuf(_myfile.rdbuf());
    std::cerr.rdbuf(_myfile.rdbuf());
  }
}
Log::~Log() {
  _myfile.close();
}
void Log::open(std::string path) {
  Log::_logpath=path;
}
void Log::close() {
  delete getLogger();
}

Log * Log::getLogger() {
  if(Log::_logger==NULL)
    Log::_logger=new Log();
  return Log::_logger;
}

void Log::log(const char * l, std::stringstream & s, const char * file, int line ){
	std::cout<<"["<< to_simple_string(microsec_clock::local_time())<< "]["<<l<< "]["<<file<<":"<<line<<"] " << s.str()<<"\r"<<std::endl;
}
void Log::log(std::stringstream & s, const char * l){
    std::cout<<"["<< to_simple_string(microsec_clock::local_time())<< "] [ "<<l<< " ] " << s.str()<<"\r"<<std::endl;
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


