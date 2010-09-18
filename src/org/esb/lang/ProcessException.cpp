/* 
 * File:   ProcessException.cpp
 * Author: HoelscJ
 * 
 * Created on 17. September 2010, 14:18
 */

#include "ProcessException.h"
namespace org {
  namespace esb {
    namespace lang {

      ProcessException::ProcessException(std::string cause):_cause(cause) {

      }

      ProcessException::~ProcessException()  throw(){

      }
      const char* ProcessException::what() const throw(){
        return _cause.c_str();
      }

    }
  }
}
