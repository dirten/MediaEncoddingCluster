/* 
 * File:   HiveClient.h
 * Author: jhoelscher
 *
 * Created on 29. Oktober 2008, 14:43
 */
#include "string"
#ifndef _HIVECLIENT_H
#define	_HIVECLIENT_H
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/signal/MessageListener.h"
#include "org/esb/signal/Message.h"

#include <boost/thread.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
namespace ipc=boost::interprocess;
namespace org{
  namespace esb{
    namespace hive{
      class HiveClient: public org::esb::signal::MessageListener{
      public:
        HiveClient(std::string host, int port);
       // HiveClient(org::esb::net::TcpSocket & sock);
		~HiveClient();
        void connect();
        void start();
        void stop();
        void process();
        void process2();
        void onMessage(org::esb::signal::Message&);
      private:
        bool _toHalt;
        std::string _host;
        int _port;
        org::esb::io::ObjectInputStream * _ois;
        org::esb::io::ObjectOutputStream * _oos;
        org::esb::net::TcpSocket * _sock;
        boost::mutex terminationMutex;
        boost::condition ctrlCHit;
		void packetReader();
		void packetWriter();
      };
    }
  }
}


#endif	/* _HIVECLIENT_H */

