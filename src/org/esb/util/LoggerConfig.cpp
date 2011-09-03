void LoggerConfig(){
    log4cplus::BasicConfigurator config;

    log4cplus::helpers::Properties & properties = const_cast<log4cplus::helpers::Properties &>(config.getProperties());
    //prop1.setProperty("rootLogger1", "%d{%m/%d/%y %H:%M:%S,%Q} [%t] %p [%c:%L] - %m%n");
    //prop1.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout"), LOG4CPLUS_TEXT("log4cplus::TTCCLayout"));
//    prop.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
    //prop.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout.ConversionPattern"), LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S,%Q} [%t] %p [%c:%L] - %m%n"));


          std::string log_pattern = "%d{%m/%d/%y %H:%M:%S,%Q} [%t] %p [%c:%L] - %m%n";
          std::string logpath=getenv("log.path")!=NULL?getenv("log.path"):"";
          std::string loglevel=getenv("loglevel")!=NULL?getenv("loglevel"):"";
          //logpath=pathenv;//org::esb::config::Config::get("log.path");
          std::string logdebug=logpath+"/mhive.log";
          std::string logwarn=logpath+"/mhive-warn.log";
          //std::cout <<"logpath:"<<logdebug<<std::endl;
          //std::cout <<"logwarn:"<<logdebug<<std::endl;
          //std::cout <<"loglevel:"<<loglevel<<std::endl;
          //org::esb::io::File logdir(logpath);
          //if(!logdir.exists())
            //logdir.mkdir();
          //logpath+="/mhive.log";
          
          properties.setProperty(LOG4CPLUS_TEXT("rootLogger"), LOG4CPLUS_TEXT("DEBUG, STDOUT"));
          
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT"), LOG4CPLUS_TEXT("log4cplus::ConsoleAppender"));
          //properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout"), LOG4CPLUS_TEXT("log4cplus::TTCCLayout"));
          //log4cplus::PropertyConfigurator::configure();
          std::cout <<" logging enabled"<<std::endl;
          //properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
          //properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.layout.ConversionPattern"), LOG4CPLUS_TEXT(log_pattern));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.1"), LOG4CPLUS_TEXT("log4cplus::spi::LogLevelRangeFilter"));

          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.1.LogLevelMin"), LOG4CPLUS_TEXT(loglevel));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.1.LogLevelMax"), LOG4CPLUS_TEXT("Fatal"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.1.AcceptOnMatch"), LOG4CPLUS_TEXT("true"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.STDOUT.filters.2"), LOG4CPLUS_TEXT("log4cplus::spi::DenyAllFilter"));
//            return;
        
          

          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN"), LOG4CPLUS_TEXT("log4cplus::RollingFileAppender"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.File"), LOG4CPLUS_TEXT(logdebug));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.MaxFileSize"), LOG4CPLUS_TEXT("50MB"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.MaxBackupIndex"), LOG4CPLUS_TEXT("10"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.MAIN.layout.ConversionPattern"), LOG4CPLUS_TEXT(log_pattern));
           // return;

          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN"), LOG4CPLUS_TEXT("log4cplus::RollingFileAppender"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.File"), LOG4CPLUS_TEXT(logwarn));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.MaxFileSize"), LOG4CPLUS_TEXT("50MB"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.MaxBackupIndex"), LOG4CPLUS_TEXT("10"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.layout"), LOG4CPLUS_TEXT("log4cplus::PatternLayout"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.layout.ConversionPattern"), LOG4CPLUS_TEXT(log_pattern));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.1"), LOG4CPLUS_TEXT("log4cplus::spi::LogLevelRangeFilter"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.1.LogLevelMin"), LOG4CPLUS_TEXT("Warn"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.1.LogLevelMax"), LOG4CPLUS_TEXT("Fatal"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.1.AcceptOnMatch"), LOG4CPLUS_TEXT("true"));
          properties.setProperty(LOG4CPLUS_TEXT("appender.WARN.filters.2"), LOG4CPLUS_TEXT("log4cplus::spi::DenyAllFilter"));
          //log4cplus::PropertyConfigurator::reconfigure();

    config.configure();


}