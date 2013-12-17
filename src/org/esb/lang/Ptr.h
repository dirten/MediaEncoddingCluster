#include "boost/shared_ptr.hpp"
#include <boost/serialization/split_free.hpp>

//#include "Poco/SharedPtr.h"
#ifndef ORG_ESB_LANG_PTR
#define ORG_ESB_LANG_PTR
#include "exports.h"
namespace org {
  namespace esb {
    namespace lang {

      template <typename T>
    class Ptr : public boost::shared_ptr<T> {
        //class Ptr : public Poco::SharedPtr<T> {
      public:

        Ptr() {
        }

        Ptr(T * elem) : boost::shared_ptr<T>(elem) {
        }

        Ptr(boost::shared_ptr<T>d):boost::shared_ptr<T>(d){}
        template<class Archive, class T2>
        inline void serialize(
            Archive & ar,
            boost::shared_ptr< T2 > &t,
            const unsigned int file_version
        ){
          boost::serialization::split_free(ar, t, file_version);
        }
      };
    }
  }
}
using namespace org::esb::lang;
#endif

