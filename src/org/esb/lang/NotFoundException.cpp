/* 
 * File:   NotFoundException.cpp
 * Author: HoelscJ
 * 
 * Created on 29. August 2011, 14:11
 */

#include "NotFoundException.h"
#include "org/esb/util/Log.h"
namespace org {
  namespace esb {
    namespace lang {

      NotFoundException::NotFoundException(std::string & msg):Exception(msg) {
        LOGERROR(msg);
      }
      NotFoundException::NotFoundException(const char * file, int line,std::string & msg):Exception(file, line,msg) {
        LOGERROR(msg);
      }

      NotFoundException::~NotFoundException()throw() {

      
      }
    }
  }
}
