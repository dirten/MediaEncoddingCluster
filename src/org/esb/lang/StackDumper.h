/* 
 * File:   StackDumper.h
 * Author: jhoelscher
 *
 * Created on 14. September 2009, 13:10
 */


#ifndef _STACKDUMPER_H
#define	_STACKDUMPER_H

#include "boost/shared_ptr.hpp"
#include "org/esb/util/Log.h"
namespace google_breakpad{
  class ExceptionHandler;
}
class StackDumper {
public:
    StackDumper(std::string dmp_path);
    ~StackDumper();
      classlogger("org.esb.lang.Stackdumper")

private:
    boost::shared_ptr<google_breakpad::ExceptionHandler> exhandler;
#ifdef __WIN32__
    bool DumpSender(const wchar_t* dump_path,
            const wchar_t* minidump_id,
            void* context,
            EXCEPTION_POINTERS* exinfo,
            MDRawAssertionInfo* assertion,
            bool succeeded);
#elif __APPLE__

#endif
};
//global function to print Program stack traces

#endif	/* _STACKDUMPER_H */

