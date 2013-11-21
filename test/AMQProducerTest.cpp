
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
  Destination* destination = session->createQueue("MHIVE.OUTPUT");
  MessageProducer*producer = session->createProducer(destination);
  producer->setDeliveryMode(DeliveryMode::PERSISTENT);

  int l=1*1024*1024;
  unsigned char * bytes=new unsigned char[l];
  for (int ix = 0; ix < 1000; ++ix) {
    //TextMessage* message(session->createTextMessage("neue nachricht"));
    BytesMessage* message(session->createBytesMessage(bytes,l));
    message->setIntProperty("Integer", ix);
    printf("Sent message #%d from thread %s\n", ix + 1, "bla");
    producer->send(message);
    delete message;
    session->commit();
  }
  connection->close();
  return 0;
}
