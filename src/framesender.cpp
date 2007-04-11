#include "framesender.h"

using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::concurrent;
using namespace cms;
using namespace std;

    Connection* connection;
    Session* session;
    Destination* destination;
    MessageProducer* producer;
    int numMessages;
    bool useTopic;
    
    FrameSender::FrameSender(){
        connection = NULL;
        session = NULL;
        destination = NULL;
        producer = NULL;
    }
    
    FrameSender::~FrameSender(){
        cleanup();
    }
    
    void FrameSender::run() {
        try {
            // Create a ConnectionFactory
            ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory("tcp://127.0.0.1:61613");

            // Create a Connection
            connection = connectionFactory->createConnection();
            connection->start();

            // Create a Session
            session = connection->createSession( Session::AUTO_ACKNOWLEDGE );
	    cout << "SessionKonstruktor :"<< session << endl;

            // Create the destination (Topic or Queue)
            if( useTopic ) {
                destination = session->createTopic( "TEST.FOO" );
            } else {
                destination = session->createQueue( "TEST.FOO" );
            }

            // Create a MessageProducer from the Session to the Topic or Queue
            producer = session->createProducer( destination );
            producer->setDeliveryMode( DeliveryMode::NON_PERSISTANT );
	    /*            
            // Create the Thread Id String
            string threadIdStr = Integer::toString( Thread::getId() );
            
            // Create a messages
            string text = (string)"Hello world! from thread " + threadIdStr;
            
            for( int ix=0; ix<numMessages; ++ix ){
                TextMessage* message = session->createTextMessage( text );

                // Tell the producer to send the message
                printf( "Sent message from thread %s\n", threadIdStr.c_str() );
                producer->send( message );
                
                delete message;
            }
            */
        }catch ( CMSException& e ) {
            e.printStackTrace();
        }
    }
    
    void FrameSender::send(const char * data){
	try{
	string text=(string)data;
        TextMessage* message = session->createTextMessage( text);
        // Tell the producer to send the message
        producer->send( message );
        delete message;
	}catch ( CMSException& e ) {
            e.printStackTrace();
        }
    
    }
    

    void FrameSender::cleanup(){
                    
            // Destroy resources.
            try{                        
                if( destination != NULL ) delete destination;
            }catch ( CMSException& e ) {}
            destination = NULL;
            
            try{
                if( producer != NULL ) delete producer;
            }catch ( CMSException& e ) {}
            producer = NULL;
            
            // Close open resources.
            try{
                if( session != NULL ) session->close();
                if( connection != NULL ) connection->close();
            }catch ( CMSException& e ) {}

            try{
                if( session != NULL ) delete session;
            }catch ( CMSException& e ) {}
            session = NULL;
            
            try{
                if( connection != NULL ) delete connection;
            }catch ( CMSException& e ) {}
            connection = NULL;
    }
