#ifndef ORG_ESB_SIGNAL_MESSAGE_H
#define ORG_ESB_SIGNAL_MESSAGE_H
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include "org/esb/lang/Ptr.h"
#include "org/esb/signal/exports.h"

#pragma warning( disable: 4251 )
namespace org {
  namespace esb {
    namespace hive {
      namespace job {
        class ProcessUnit;
      }
    }
    namespace signal {

      class SIGNAL_EXPORT Message {
      public:
        Message();
        Message(boost::shared_ptr<void *> ent);

        Message & setProperty(std::string key, boost::any value);

        template<class T>
        T getProperty(std::string key) {
          return boost::any_cast<T > (str_props[key]);
        }

        bool containsProperty(std::string key);
        ~Message();
      private:
        std::map<std::string, boost::any> str_props;
      };
    }
  }
}
#endif


