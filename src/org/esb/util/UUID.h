/* 
 * File:   UUID.h
 * Author: HoelscJ
 *
 * Created on 26. Oktober 2011, 17:02
 */

#ifndef UUID_H
#define	UUID_H
#include <string>
#include "exports.h"
namespace org {
  namespace esb {
    namespace util {

      class UTIL_EXPORT UUID {
      public:
        UUID();
        virtual ~UUID();
        operator std::string() const { return _uuid; }
      private:
        std::string _uuid;
      };
    }
  }
}

#endif	/* UUID_H */

