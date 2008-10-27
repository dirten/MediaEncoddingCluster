#include "Message.h"

namespace org{
namespace esb{
namespace signal{

Message::Message(){

}
Message::Message(boost::shared_ptr<void *> ent){

}

Message & Message::setProperty(std::string key, std::string value){
  props[key]=value;
  return *this;
}

std::string & Message::getProperty(std::string key){
  return props[key];
}

bool Message::containsProperty(std::string key){
  return true;
}


}}}

