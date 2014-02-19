#include "Log.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include "org/esb/io/File.h"
#include "Poco/ConsoleChannel.h"


using namespace boost::posix_time;

Log::Log() {
}

Log::~Log() {
}
void Log::open(std::string path) {
  std::cout << "enable logging to "<<path << std::endl;
  /*initializing new logging*/
  Poco::ConsoleChannel * consoleChannel=new Poco::ConsoleChannel();

  Poco::PatternFormatter * formater=new Poco::PatternFormatter();
  formater->setProperty("pattern","%Y-%m-%d %H:%M:%S.%i [%I][%p] %s: %t");

  Poco::FormattingChannel * formatChannel=new Poco::FormattingChannel(formater, consoleChannel);

  Logger::root().setChannel(formatChannel);
  Logger::root().setLevel("trace");

}

void Log::close() {

}

Log * Log::getLogger(std::string logger) {
  return NULL;
}


void Log::log(std::string l, std::stringstream & s, const char * file, int line) {
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


