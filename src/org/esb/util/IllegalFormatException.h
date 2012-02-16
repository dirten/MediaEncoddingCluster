#ifndef ORG_ESB_SQL_SQLEXCEPTION_H
#define ORG_ESB_SQL_SQLEXCEPTION_H

#include <string.h>
#include <stdexcept>
#include "Log.h"
#include "exports.h"

namespace org {
  namespace esb {
    namespace util {

      class UTIL_EXPORT IllegalFormatException : public std::runtime_error {
        classlogger("org.esb.util.IllegalFormatException");
      public:
        explicit IllegalFormatException(std::string & msg);
        explicit IllegalFormatException(const char * msg);

      };
    }
  }
}
#endif

