/* 
 * File:   StackDumper.h
 * Author: jhoelscher
 *
 * Created on 14. September 2009, 13:10
 */


#ifndef _STACKDUMPER_H
#define	_STACKDUMPER_H

#ifndef __WIN32__
#ifdef __APPLE__
#include "client/mac/handler/exception_handler.h"
#else
#include "client/linux/handler/exception_handler.h"
#include "common/linux/google_crashdump_uploader.h"
#endif
#elif defined __WIN32__
#include "client/windows/handler/exception_handler.h"
#include "client/windows/sender/crash_report_sender.h"
#endif
#include "org/esb/util/Log.h"
class StackDumper {
public:
    StackDumper(std::string dmp_path);
    ~StackDumper();
      classlogger("org.esb.lang.Stackdumper")

private:
    google_breakpad::ExceptionHandler exhandler;
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

