
#include "activemq/library/ActiveMQCPP.h"
#include "cms/Connection.h"
#include "activemq/core/ActiveMQConnectionFactory.h"

#include <cstdio>
using namespace activemq::core;
using namespace cms;
int main(int argc, char ** argv){

  activemq::library::ActiveMQCPP::initializeLibrary();

  //ConnectionFactory* connectionFactory(ConnectionFactory::createCMSConnectionFactory("tcp://lngmued000197:61616"));
  ConnectionFactory* connectionFactory(ConnectionFactory::createCMSConnectionFactory("tcp://127.0.0.1:61616"));
  Connection*connection = connectionFactory->createConnection();
  connection->start();

  Session*session = connection->createSession(Session::SESSION_TRANSACTED);
  //Session*session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
  Destination* destination = session->createQueue("TEST.FOO");
  MessageProducer*producer = session->createProducer(destination);
  producer->setDeliveryMode(DeliveryMode::PERSISTENT);


  for (int ix = 0; ix < 1000; ++ix) {
    TextMessage* message(session->createTextMessage("neue nachricht"));
    message->setIntProperty("Integer", ix);
    printf("Sent message #%d from thread %s\n", ix + 1, "bla");
    producer->send(message);
    delete message;
  }
  session->commit();
  connection->close();
  return 0;
}
