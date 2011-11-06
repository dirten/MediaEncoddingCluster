/* 
 * File:   ApiWebserverTest.cpp
 * Author: HoelscJ
 *
 * Created on 30. August 2011, 13:04
 */

#include <cstdlib>
#include <gloox/connectionlistener.h>
#include <gloox/client.h>

#include <gloox/loghandler.h>
#include <gloox/logsink.h>
#include <gloox/disco.h>

using namespace std;



class ConnectTest: public gloox::ConnectionListener, gloox::LogHandler{
    public:
	ConnectTest(){
	    gloox::JID jid("test@macbook.local");
	    j=new gloox::Client(jid, "test");
	    j->disco()->setIdentity( "client", "bot" );

	    j->logInstance().registerLogHandler( gloox::LogLevelDebug, gloox::LogAreaAll, this );
	    if( j->connect( false ) )
      {
        gloox::ConnectionError ce = gloox::ConnNoError;
        while( ce == gloox::ConnNoError )
        {
          ce = j->recv();
        }
        printf( "ce: %d\n", ce );
      }

	}
	virtual void handleLog( gloox::LogLevel level, gloox::LogArea area, const std::string& message ){
    	    printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
	}
    virtual void onConnect()
    {
      printf( "connected!!!\n" );
    }

    virtual void onDisconnect( gloox::ConnectionError e )
    {
      printf( "message_test: disconnected: %d\n", e );
      if( e == gloox::ConnAuthenticationFailed )
        printf( "auth failed. reason: %d\n", j->authError() );
    }

    virtual bool onTLSConnect( const gloox::CertInfo& info )
    {
      time_t from( info.date_from );
      time_t to( info.date_to );

      printf( "status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
              "from: %s\nto: %s\n",
              info.status, info.issuer.c_str(), info.server.c_str(),
              info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
              info.compression.c_str(), ctime( &from ), ctime( &to ) );
      return true;
    }

    private:
	gloox::Client * j;
};
/*
 * 
 */
int main(int argc, char** argv) {
    ConnectTest t;
  return 0;
}

