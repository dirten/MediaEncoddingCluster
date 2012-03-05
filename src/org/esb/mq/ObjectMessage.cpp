/*
 * File:   QueueMessage.cpp
 * Author: HoelscJ
 * 
 * Created on 17. Dezember 2010, 11:55
 */

#include "ObjectMessage.h"
namespace org {
  namespace esb {
    namespace mq {

      ObjectMessage::ObjectMessage():safmq::QueueMessage() {

      }

      ObjectMessage::~ObjectMessage() {
      }
    }
  }
}
