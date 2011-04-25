#ifndef ORG_ESB_SIGNAL_MESSAGE_H
#define ORG_ESB_SIGNAL_MESSAGE_H
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include "org/esb/signal/exports.h"
namespace org{
namespace esb{
  namespace hive{
    namespace job{
      class ProcessUnit;
    }
  }
namespace signal{
class SIGNAL_EXPORT Message{
  public:
    Message();
    Message(boost::shared_ptr<void *> ent);
    Message & setProperty(std::string key, std::string value);
    Message & setProperty(std::string key, int value);
    Message & setProperty(std::string key, boost::shared_ptr<org::esb::hive::job::ProcessUnit> );
    std::string & getProperty(std::string key);
    boost::shared_ptr<org::esb::hive::job::ProcessUnit> getPtrProperty(std::string key);
    bool containsProperty(std::string key);
	~Message();
private:
    std::map<std::string, std::string> str_props;
    std::map<std::string, std::string> int_props;
    std::map<std::string, boost::shared_ptr<org::esb::hive::job::ProcessUnit> > void_props;

};
}}}
#endif


