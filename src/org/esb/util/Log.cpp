#include "Log.h"

Log * Log::_logger=NULL;

Log::Log() {
  myfile.open("data.log", std::ios::out | std::ios::app);
}

Log * Log::getLogger() {
  if(Log::_logger==NULL)
    Log::_logger=new Log();
  return Log::_logger;
}

void Log::loggg(std::stringstream & s, const char * l){
  std::cout<< "[ "<<l<< " ] " << s.str()<<"\r"<<std::endl;
}

void Log::debug(std::stringstream & s){
  loggg(s,"debug");
}
void Log::info(std::stringstream & s){
  loggg(s,"info");
}
void Log::warn(std::stringstream & s){
  loggg(s,"warn");
}
void Log::error(std::stringstream & s){
  loggg(s,"error");
}
void Log::fatal(std::stringstream & s){
  loggg(s,"fatal");
}


