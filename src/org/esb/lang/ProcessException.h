/* 
 * File:   ProcessException.h
 * Author: HoelscJ
 *
 * Created on 17. September 2010, 14:18
 */

#ifndef PROCESSEXCEPTION_H
#define	PROCESSEXCEPTION_H
#include <exception>
#include <string>
#include "exports.h"
namespace org {
  namespace esb {
    namespace lang {

      class LANG_EXPORT ProcessException:public std::exception {
      public:
        ProcessException(std::string cause);
        
        virtual ~ProcessException() throw();
        const char* what() const throw();
      private:
        std::string _cause;

      };
    }
  }
}

#endif	/* PROCESSEXCEPTION_H */

