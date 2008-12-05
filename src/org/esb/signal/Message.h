#ifndef ORG_ESB_SIGNAL_MESSAGE_H
#define ORG_ESB_SIGNAL_MESSAGE_H
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
namespace org{
namespace esb{
namespace signal{
class Message{
  public:
    Message();
    Message(boost::shared_ptr<void *> ent);
    Message & setProperty(std::string key, std::string value);
    Message & setProperty(std::string key, int value);
    std::string & getProperty(std::string key);
    bool containsProperty(std::string key);
  private:
    std::map<std::string, std::string> str_props;
    std::map<std::string, std::string> int_props;
};
}}}
#endif


