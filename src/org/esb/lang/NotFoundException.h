/* 
 * File:   NotFoundException.h
 * Author: HoelscJ
 *
 * Created on 29. August 2011, 14:11
 */

#ifndef NOTFOUNDEXCEPTION_H
#define	NOTFOUNDEXCEPTION_H
#include "Exception.h"
#include "exports.h"

namespace org {
  namespace esb {
    namespace lang {

      class LANG_EXPORT NotFoundException:public Exception {
        
      public:
        explicit NotFoundException(std::string & msg);
        explicit NotFoundException(const char * file, int line,std::string & msg);
        virtual ~NotFoundException() throw();
      private:

      };
    }
  }
}

#endif	/* NOTFOUNDEXCEPTION_H */

