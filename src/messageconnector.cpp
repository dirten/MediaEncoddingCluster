/***************************************************************************
 *   Copyright (C) 2007 by Jan Hoelscher                                   *
 *   jan.hoelscher@esblab.com                                              *
 ***************************************************************************/
#include <activemq/concurrent/Thread.h>
#include <activemq/concurrent/Runnable.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Integer.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <iostream>
#include "messageconnector.h"

using namespace activemq::core;
using namespace activemq::util;
using namespace activemq::concurrent;
using namespace cms;
using namespace std;

    
    HelloWorldProducer::HelloWorldProducer( int numMessages, bool useTopic = false ){
        connection = NULL;
        session = NULL;
        destination = NULL;
        producer = NULL;
        this->numMessages = numMessages;
        this->useTopic = useTopic;
    }
    
    HelloWorldProducer::~HelloWorldProducer(){
        cleanup();
    }
    
    void HelloWorldProducer::run() {
        try {
            // Create a ConnectionFactory
            ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory("tcp://127.0.0.1:61613");

            // Create a Connection
            connection = connectionFactory->createConnection();
            connection->start();

            // Create a Session
            session = connection->createSession( Session::AUTO_ACKNOWLEDGE );

            // Create the destination (Topic or Queue)
            if( useTopic ) {
                destination = session->createTopic( "TEST.FOO" );
            } else {
                destination = session->createQueue( "TEST.FOO" );
            }

            // Create a MessageProducer from the Session to the Topic or Queue
            producer = session->createProducer( destination );
            producer->setDeliveryMode( DeliveryMode::NON_PERSISTANT );
            
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
            
        }catch ( CMSException& e ) {
            e.printStackTrace();
        }
    }
    


    void HelloWorldProducer::cleanup(){
                    
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
