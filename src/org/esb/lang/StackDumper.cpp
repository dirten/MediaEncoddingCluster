#include <string>

//#include <curl/curl.h>

#include "StackDumper.h"
#ifndef __WIN32__
#ifdef __APPLE__
#include "client/mac/handler/exception_handler.h"
#else
#include "client/linux/handler/exception_handler.h"
//#include "common/linux/google_crashdump_uploader.h"
#endif
#elif defined __WIN32__
#include "client/windows/handler/exception_handler.h"
#endif

#include "org/esb/util/Log.h"


StackDumper::StackDumper(std::string dmp_path) 
 {
exhandler=boost::shared_ptr<google_breakpad::ExceptionHandler>(new google_breakpad::ExceptionHandler(
#ifndef __WIN32__
dmp_path,
#elif defined __WIN32__
std::wstring(dmp_path.begin(), dmp_path.end()),
#endif
NULL,
        NULL,
//#if defined(__LINUX__) || defined(__WIN32__)
//&MyDumpSender,
//#else
//&MyDumpSender,
//#endif
NULL,
#ifndef __WIN32__
true
#elif defined __WIN32__
google_breakpad::ExceptionHandler::HANDLER_ALL
#endif
#if defined(__APPLE__)
,NULL
#endif
));
}

StackDumper::~StackDumper() {

}

