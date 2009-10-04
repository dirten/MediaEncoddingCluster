/* 
 * File:   StackDumper.h
 * Author: jhoelscher
 *
 * Created on 14. September 2009, 13:10
 */


#ifndef _STACKDUMPER_H
#define	_STACKDUMPER_H
#ifndef WIN32
	#ifdef __APPLE__
		#include "client/mac/handler/exception_handler.h"
	#else
		#include "client/linux/handler/exception_handler.h"
#endif
#elif WIN32
#include "client/windows/handler/exception_handler.h"
#include "client/windows/sender/crash_report_sender.h"
#endif
class StackDumper{
public:
    StackDumper(std::string dmp_path);
    ~StackDumper();
private:
    google_breakpad::ExceptionHandler exhandler;
#ifdef WIN32
    bool DumpSender(const wchar_t* dump_path,
                                   const wchar_t* minidump_id,
                                   void* context,
                                   EXCEPTION_POINTERS* exinfo,
                                   MDRawAssertionInfo* assertion,
                                   bool succeeded);
#endif
};
//global function to print Program stack traces

#endif	/* _STACKDUMPER_H */

