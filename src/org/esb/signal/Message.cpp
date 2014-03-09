#include "Message.h"

#include "org/esb/hive/job/ProcessUnit.h"
namespace org {
  namespace esb {
    namespace signal {

      Message::Message() {

      }

      Message & Message::setProperty(std::string key, boost::any value){
        str_props[key] = value;
        return *this;
      }

      Message::Message(boost::shared_ptr<void *> ent) {

      }

      Message::~Message() {

      }

      bool Message::containsProperty(std::string key) {
        return str_props.count(key);
      }

    }
  }
}

