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
      //POCO_DECLARE_EXCEPTION(CORE_EXPORT, TaskException, Poco::Exception);

      class TaskException : public Poco::Exception {
      public:
        TaskException(int code = 0);
        TaskException(const std::string& msg, int code = 0);
        TaskException(const std::string& msg, const std::string& arg, int code = 0);
        TaskException(const std::string& msg, const Poco::Exception& exc, int code = 0);
        TaskException(const TaskException& exc);
        ~TaskException() throw ();
        TaskException& operator =(const TaskException& exc);
        const char* name() const throw ();
        const char* className() const throw ();
        Poco::Exception* clone() const;
        void rethrow() const;

        const char * what()const throw();
      private:

      };
    }
  }
}

#endif	/* TASKEXCEPTION_H */

