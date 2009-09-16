/* 
 * File:   StackDumper.h
 * Author: jhoelscher
 *
 * Created on 14. September 2009, 13:10
 */


#ifndef _STACKDUMPER_H
#define	_STACKDUMPER_H
#ifndef WIN32
#include "client/linux/handler/exception_handler.h"
#elif WIN32
#include "client/windows/handler/exception_handler.h"
#endif
class StackDumper{
public:
    StackDumper();
    ~StackDumper();
private:
    google_breakpad::ExceptionHandler exhandler;
};
//global function to print Program stack traces

#endif	/* _STACKDUMPER_H */

