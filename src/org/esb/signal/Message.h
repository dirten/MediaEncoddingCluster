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
        //Message & setProperty(std::string key, std::string value);
        //Message & setProperty(std::string key, int value);
        Message & setProperty(std::string key, boost::any value);
        //Message & setProperty(std::string key, boost::shared_ptr<org::esb::hive::job::ProcessUnit>);
        //Message & setProperty(std::string key, boost::shared_ptr<void>);
        //std::string & getProperty(std::string key);

        template<typename T>
        T getProperty(std::string key) {
          //if (containsProperty(key))
          return boost::any_cast<T > (str_props[key]);
        }

        template<typename T>
        boost::shared_ptr<T> getPtrProperty(std::string key) {
          return boost::static_pointer_cast<T > (void_props[key]);
        }
        bool containsProperty(std::string key);
        ~Message();
      private:
        std::map<std::string, boost::any> str_props;
        std::map<std::string, std::string> int_props;
        std::map<std::string, boost::shared_ptr<org::esb::hive::job::ProcessUnit> > pu_props;
        std::map<std::string, boost::shared_ptr<void> > void_props;

      };
    }
  }
}
#endif


