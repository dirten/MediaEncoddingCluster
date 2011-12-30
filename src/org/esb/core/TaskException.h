/* 
 * File:   TaskException.h
 * Author: HoelscJ
 *
 * Created on 19. Dezember 2011, 12:51
 */

#ifndef TASKEXCEPTION_H
#define	TASKEXCEPTION_H
#include "Poco/Exception.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {
      POCO_DECLARE_EXCEPTION(CORE_EXPORT, TaskException, Poco::Exception);
      /*
      class TaskException {
      public:
        TaskException();
        TaskException(const TaskException& orig);
        virtual ~TaskException();
      private:

      };*/
    }
  }
}

#endif	/* TASKEXCEPTION_H */

