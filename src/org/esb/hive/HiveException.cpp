/* 
 * File:   HiveException.cpp
 * Author: HoelscJ
 * 
 * Created on 22. September 2010, 18:31
 */

#include <sstream>

#include "HiveException.h"
namespace org {
  namespace esb {
    namespace hive {

      HiveException::HiveException(std::string what):_what(what) {
        
      }

      HiveException::HiveException(const HiveException& orig) {
      }

      HiveException::~HiveException() throw(){
      }

      const char * HiveException::what() throw () {
        return "";
      }
    }
  }
}
