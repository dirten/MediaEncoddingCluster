


#include "activemq/library/ActiveMQCPP.h"
#include "cms/Connection.h"
#include "activemq/core/ActiveMQConnectionFactory.h"
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <cstdio>
using namespace activemq::core;
using namespace decaf::lang;
using namespace cms;
using namespace std;

class MessageHandler : public ExceptionListener,public MessageListener,public Runnable  {
    Connection* connection;
    Session* session;
    Destination* destination;
    MessageConsumer* consumer;
  public:
    MessageHandler(){

    }

    virtual void run() {

      try {


        // Create a ConnectionFactory
        ConnectionFactory * connectionFactory(ConnectionFactory::createCMSConnectionFactory("failover:tcp://127.0.0.1:61616?jms.prefetchPolicy.queuePrefetch=1"));

        // Create a Connection
        connection = connectionFactory->createConnection();
        //connection->setDispatchAsync(false);
        connection->start();
        connection->setExceptionListener(this);

        // Create a Session
        session = connection->createSession(Session::SESSION_TRANSACTED);
        destination = session->createQueue("MHIVE.OUTPUT?consumer.prefetchSize=1");

        // Create a MessageConsumer from the Session to the Topic or Queue
        consumer = session->createConsumer(destination);

        consumer->setMessageListener(this);

        std::cout.flush();
        std::cerr.flush();

        // Indicate we are ready for messages.
        //latch.countDown();

        // Wait while asynchronous messages come in.
        //doneLatch.await(waitMillis);

      } catch (CMSException& e) {
        // Indicate we are ready for messages.
        // latch.countDown();
        e.printStackTrace();
      }
    }
    virtual void onMessage(const Message* message) {

      static int count = 0;

      try {
        count++;
        const TextMessage* textMessage = dynamic_cast<const TextMessage*> (message);
        string text = "";

        if (textMessage != NULL) {
          text = textMessage->getText();
        } else {
          text = "NOT A TEXTMESSAGE!";
        }
        const BytesMessage* bytesMessage = dynamic_cast<const BytesMessage*> (message);
        int length=0;
        if(bytesMessage!=NULL){
          length=bytesMessage->getBodyLength ();
        }
        printf("Message #%d Received bytes: %d\n", count, length);



      } catch (CMSException& e) {
        e.printStackTrace();
      }

      // Commit all messages.

      session->commit();


      // No matter what, tag the count down latch until done.
      //doneLatch.countDown();
    }

    virtual void onException(const CMSException& ex ) {
      printf("CMS Exception occurred.  Shutting down client.\n");
      ex.printStackTrace();
      //exit(1);
    }

};

int main(int argc, char ** argv){
  activemq::library::ActiveMQCPP::initializeLibrary();
  MessageHandler *handler=new MessageHandler();
  Thread thread(handler);
  thread.start();
  thread.sleep(1000*10000);
  return 0;
}

