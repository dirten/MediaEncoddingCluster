/* 
 * File:   GraphException.h
 * Author: jhoelscher
 *
 * Created on 12. Dezember 2011, 10:41
 */

#ifndef GRAPHEXCEPTION_H
#define	GRAPHEXCEPTION_H

#include "org/esb/lang/Exception.h"
#include "exports.h"

namespace org {
  namespace esb {
    namespace core {

      class CORE_EXPORT GraphException:public org::esb::lang::Exception {
      public:
        GraphException(std::string, std::string el="");
        virtual ~GraphException() throw();
        std::string getElementId();
      private:
        std::string _element;
      };
    }
  }
}
#endif	/* GRAPHEXCEPTION_H */

