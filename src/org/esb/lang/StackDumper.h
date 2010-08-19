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
};
//global function to print Program stack traces

#endif	/* _STACKDUMPER_H */

