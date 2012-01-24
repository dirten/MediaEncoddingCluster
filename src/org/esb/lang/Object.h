/* 
 * File:   Object.h
 * Author: HoelscJ
 *
 * Created on 10. März 2011, 11:19
 */

#ifndef OBJECT_H
#define	OBJECT_H
#include "Poco/RefCountedObject.h"
#include "exports.h"
namespace org {
  namespace esb {
    namespace lang {
      

      template <typename T=std::string>
      class LANG_EXPORT Object : public Poco::RefCountedObject {
      public:

        Object():RefCountedObject(){}
        virtual ~Object(){}
      private:
      };
    }
  }
}

#endif	/* OBJECT_H */

