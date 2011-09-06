/* 
 * File:   LogTest.cpp
 * Author: jhoelscher
 *
 * Created on 3. September 2011, 13:40
 */

#include <cstdlib>
#include "org/esb/util/Log.h"
#include "org/esb/util/LoggerConfig.cpp"
#include "org/esb/util/Foreach.h"


using namespace std;

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <iostream>

using namespace std;
//using namespace log4cplus;



log4cplus::Logger logger = log4cplus::Logger::getInstance("main");
void printMessages()
{
  
    LOG4CPLUS_TRACE(logger, "printMessages()");
    LOG4CPLUS_DEBUG(logger, "This is a DEBUG message");
    LOG4CPLUS_INFO(logger, "This is a INFO message");
    LOG4CPLUS_WARN(logger, "This is a WARN message");
    LOG4CPLUS_ERROR(logger, "This is a ERROR message");
    LOG4CPLUS_FATAL(logger, "This is a FATAL message");
}

void config(){
    log4cplus::BasicConfigurator config;

    log4cplus::helpers::Properties & prop1 = const_cast<log4cplus::helpers::Properties &>(config.getProperties());
    //prop1.setProperty("rootLogger1", "%d{%m/%d/%y %H:%M:%S,%Q} [%t] %p [%c:%L] - %m%n");
    //prop1.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout"), LOG4CPLUS_TEXT("log4cplus::TTCCLayout"));
    prop1.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
    prop1.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout.ConversionPattern"), LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S,%Q} [%t] %p [%c:%L] - %m%n"));

    config.configure();

}
int
main()
{
  {
    //LoggerConfig();
  }
  Log::open("/home/jan.hoelscher/devel/mec/res");
  LOGDEBUG("twest");

    logger.setLogLevel(log4cplus::TRACE_LOG_LEVEL);
    cout << "*** calling printMessages() with TRACE set: ***" << endl;
    printMessages();

    logger.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
    cout << "\n*** calling printMessages() with DEBUG set: ***" << endl;
    printMessages();

    logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
    cout << "\n*** calling printMessages() with INFO set: ***" << endl;
    printMessages();

    logger.setLogLevel(log4cplus::WARN_LOG_LEVEL);
    cout << "\n*** calling printMessages() with WARN set: ***" << endl;
    printMessages();

    logger.setLogLevel(log4cplus::ERROR_LOG_LEVEL);
    cout << "\n*** calling printMessages() with ERROR set: ***" << endl;
    printMessages();

    logger.setLogLevel(log4cplus::FATAL_LOG_LEVEL);
    cout << "\n*** calling printMessages() with FATAL set: ***" << endl;
    printMessages();


  //log4cplus::Logger::shutdown();
    return 0;
}

/*
class logtest{
public:
  logtest(){
    LOGDEBUG("aaahhh");
  }
};

int main(int argc, char** argv) {
  //log4cplus::BasicConfigurator::doConfigure();
  std::string bla="test";
  Log::open(bla);
  LOG4CPLUS_DEBUG(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("global")),"test");
  LOGDEBUG("test log debug");
  logtest t;
  return 0;
}
*/
