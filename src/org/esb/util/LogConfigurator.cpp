#include "org/esb/config/config.h"
#include "org/esb/io/File.h"
namespace org {
  namespace esb {
    namespace util {

      class LogConfigurator : public log4cplus::PropertyConfigurator {
      public:

        LogConfigurator() : log4cplus::PropertyConfigurator(LOG4CPLUS_TEXT(""), log4cplus::Logger::getDefaultHierarchy()) {
          std::string log_pattern = "%d{%m/%d/%y %H:%M:%S,%Q} [%t] %p [%c:%L] - %m%n";
          std::string logpath=org::esb::config::Config::get("log.path");
          org::esb::io::File logdir(logpath);
          if(!logdir.exists())
            logdir.mkdir();

          //logpath+="/mhive.log";
          
          properties.setProperty(LOG4CPLUS_TEXT("rootLogger"), LOG4CPLUS_TEXT("TRACE, STDOUT, MAIN, WARN"));
          
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT"), LOG4CPLUS_TEXT("log4cplus::ConsoleAppender"));
          //properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout"), LOG4CPLUS_TEXT("log4cplus::TTCCLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout.ConversionPattern"), LOG4CPLUS_TEXT(log_pattern));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.1"), LOG4CPLUS_TEXT("log4cplus::spi::LogLevelRangeFilter"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.1.LogLevelMin"), LOG4CPLUS_TEXT("Trace"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.1.LogLevelMax"), LOG4CPLUS_TEXT("Fatal"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.1.AcceptOnMatch"), LOG4CPLUS_TEXT("true"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.2"), LOG4CPLUS_TEXT("log4cplus::spi::DenyAllFilter"));
          

          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN"), LOG4CPLUS_TEXT("log4cplus::RollingFileAppender"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.File"), LOG4CPLUS_TEXT(logpath+"/mhive.log"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.MaxFileSize"), LOG4CPLUS_TEXT("50MB"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.MaxBackupIndex"), LOG4CPLUS_TEXT("10"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.layout.ConversionPattern"), LOG4CPLUS_TEXT(log_pattern));

          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN"), LOG4CPLUS_TEXT("log4cplus::RollingFileAppender"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.File"), LOG4CPLUS_TEXT(logpath+"/mhive-warn.log"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.MaxFileSize"), LOG4CPLUS_TEXT("50MB"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.MaxBackupIndex"), LOG4CPLUS_TEXT("10"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.layout.ConversionPattern"), LOG4CPLUS_TEXT(log_pattern));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.1"), LOG4CPLUS_TEXT("log4cplus::spi::LogLevelRangeFilter"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.1.LogLevelMin"), LOG4CPLUS_TEXT("Warn"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.1.LogLevelMax"), LOG4CPLUS_TEXT("Fatal"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.1.AcceptOnMatch"), LOG4CPLUS_TEXT("true"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.2"), LOG4CPLUS_TEXT("log4cplus::spi::DenyAllFilter"));

          /*
          //          properties.setProperty(LOG4CPLUS_TEXT("logger.DEBUG"), LOG4CPLUS_TEXT("DEBUG"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.DEBUG"), LOG4CPLUS_TEXT("log4cplus::RollingFileAppender"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.DEBUG.File"), LOG4CPLUS_TEXT("mhive.log"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.DEBUG.MaxFileSize"), LOG4CPLUS_TEXT("5MB"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.DEBUG.MaxBackupIndex"), LOG4CPLUS_TEXT("10"));
          //          properties.setProperty(LOG4CPLUS_TEXT("appender.DEBUG.layout"),LOG4CPLUS_TEXT("log4cplus::TTCCLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.DEBUG.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.DEBUG.layout.ConversionPattern"), LOG4CPLUS_TEXT(log_pattern));

          properties.setProperty(LOG4CPLUS_TEXT("additivity.DEBUG"), LOG4CPLUS_TEXT("false"));

          properties.setProperty(LOG4CPLUS_TEXT("logger.trace"), LOG4CPLUS_TEXT("TRACE, DEBUG"));
          properties.setProperty(LOG4CPLUS_TEXT("logger.org"), LOG4CPLUS_TEXT("DEBUG, DEBUG"));
          properties.setProperty(LOG4CPLUS_TEXT("logger.org.esb.hive"), LOG4CPLUS_TEXT("ERROR, ERROR"));

          properties.setProperty(LOG4CPLUS_TEXT("appender.ERROR"), LOG4CPLUS_TEXT("log4cplus::RollingFileAppender"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.ERROR.File"), LOG4CPLUS_TEXT("mhive-error.log"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.ERROR.MaxFileSize"), LOG4CPLUS_TEXT("5MB"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.ERROR.MaxBackupIndex"), LOG4CPLUS_TEXT("10"));
          //          properties.setProperty(LOG4CPLUS_TEXT("appender.DEBUG.layout"),LOG4CPLUS_TEXT("log4cplus::TTCCLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.ERROR.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.ERROR.layout.ConversionPattern"),
              LOG4CPLUS_TEXT(log_pattern));
          //          properties.setProperty(LOG4CPLUS_TEXT("additivity.ERROR"), LOG4CPLUS_TEXT("false"));

*/
          /*
                    properties.setProperty(LOG4CPLUS_TEXT("appender.org"), LOG4CPLUS_TEXT("log4cplus::RollingFileAppender"));
                    properties.setProperty(LOG4CPLUS_TEXT("appender.org.File"), LOG4CPLUS_TEXT("mhive-error.log"));
                    properties.setProperty(LOG4CPLUS_TEXT("appender.org.MaxFileSize"), LOG4CPLUS_TEXT("5MB"));
                    properties.setProperty(LOG4CPLUS_TEXT("appender.org.MaxBackupIndex"), LOG4CPLUS_TEXT("10"));
                    //          properties.setProperty(LOG4CPLUS_TEXT("appender.DEBUG.layout"),LOG4CPLUS_TEXT("log4cplus::TTCCLayout"));
                    properties.setProperty(LOG4CPLUS_TEXT("appender.org.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
                    properties.setProperty(LOG4CPLUS_TEXT("appender.org.layout.ConversionPattern"),
                        LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S,%Q} [%t] %p [%l] - %m%n"));*/
        }
        void reconfigure(){
          properties.setProperty(LOG4CPLUS_TEXT("rootLogger"), LOG4CPLUS_TEXT("INFO, MAIN"));
        }

      };
    }
  }
}
