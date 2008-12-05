#include "Message.h"

namespace org{
namespace esb{
namespace signal{

Message::Message(){

}
Message::Message(boost::shared_ptr<void *> ent){

}

Message & Message::setProperty(std::string key, std::string value){
  str_props[key]=value;
  return *this;
}
Message & Message::setProperty(std::string key, int value){
  int_props[key]=value;
  return *this;
}

std::string & Message::getProperty(std::string key){
  return str_props[key];
}

bool Message::containsProperty(std::string key){
  return true;
}


}}}

