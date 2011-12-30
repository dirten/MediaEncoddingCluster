/* 
 * File:   TaskException.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Dezember 2011, 12:51
 */

#include <typeinfo>
#include "TaskException.h"

namespace org {
  namespace esb {
    namespace core {
      POCO_IMPLEMENT_EXCEPTION(TaskException,Poco::Exception,"TaskException");
      /*
      TaskException::TaskException() {
      }

      TaskException::TaskException(const TaskException& orig) {
      }

      TaskException::~TaskException() {
      }*/
      /*
      char * TaskException::what() {
        return displayText().c_str();
      }*/
    }
  }
}