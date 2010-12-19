/* 
 * File:   QueueException.cpp
 * Author: HoelscJ
 * 
 * Created on 17. Dezember 2010, 14:15
 */

#include "QueueException.h"
namespace org {
  namespace esb {
    namespace mq {

      QueueException::QueueException(std::string what):org::esb::lang::Exception(what) {
      }

      QueueException::~QueueException() throw(){
      }
    }
  }
}
