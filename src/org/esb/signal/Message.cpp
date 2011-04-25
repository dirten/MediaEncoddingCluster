#include "Message.h"

#include "org/esb/hive/job/ProcessUnit.h"
namespace org{
namespace esb{
namespace signal{

Message::Message(){

}
Message & Message::setProperty(std::string key, boost::shared_ptr<org::esb::hive::job::ProcessUnit> val){
 void_props[key]=val;
 return *this;
}

boost::shared_ptr<org::esb::hive::job::ProcessUnit> Message::getPtrProperty(std::string key){
  return void_props[key];
}

Message::Message(boost::shared_ptr<void *> ent){

}
Message::~Message(){

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

