/* 
 * File:   Base64.h
 * Author: HoelscJ
 *
 * Created on 17. Oktober 2011, 17:55
 */

#ifndef BASE64_H
#define	BASE64_H
#include <string>
#include "exports.h"
namespace org {
  namespace esb {
    namespace util {

      class UTIL_EXPORT Base64 {
      public:
        static std::string decode(std::string);
        static std::string encode(std::string);
      private:
        Base64();
        virtual ~Base64();
      };
    }
  }
}

#endif	/* BASE64_H */

