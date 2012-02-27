#include "boost/shared_ptr.hpp"

#ifndef ORG_ESB_LANG_PTR
#define ORG_ESB_LANG_PTR
#include "exports.h"
namespace org {
  namespace esb {
    namespace lang {

      template <typename T>
      class Ptr : public boost::shared_ptr<T> {
      public:

        Ptr() {
        }

        Ptr(T * elem) : boost::shared_ptr<T>(elem) {
        }
        Ptr(boost::shared_ptr<T>d):boost::shared_ptr<T>(d){}
      };
    }
  }
}
using namespace org::esb::lang;
#endif

