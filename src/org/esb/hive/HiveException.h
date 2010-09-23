/* 
 * File:   HiveException.h
 * Author: HoelscJ
 *
 * Created on 22. September 2010, 18:31
 */

#ifndef HIVEEXCEPTION_H
#define	HIVEEXCEPTION_H
#include <exception>
#include <string>
namespace org {
  namespace esb {
    namespace hive {

      class HiveException : public std::exception {
      public:
        HiveException(std::string what);
        HiveException(const HiveException& orig);
        virtual ~HiveException() throw();
        const char * what() throw();
      private:
        std::string _what;

      };
    }
  }
}

#endif	/* HIVEEXCEPTION_H */

