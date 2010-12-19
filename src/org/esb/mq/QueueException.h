/* 
 * File:   QueueException.h
 * Author: HoelscJ
 *
 * Created on 17. Dezember 2010, 14:15
 */

#ifndef QUEUEEXCEPTION_H
#define	QUEUEEXCEPTION_H
#include "org/esb/lang/Exception.h"
namespace org {
  namespace esb {
    namespace mq {

      class QueueException: public org::esb::lang::Exception{
      public:
        QueueException(std::string what);
        virtual ~QueueException() throw();
      private:

      };
    }
  }
}

#endif	/* QUEUEEXCEPTION_H */

