#include "HiveClient.h"
#include "org/esb/util/Log.h"
#include "org/esb/net/TcpSocket.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/lang/Thread.h"
namespace org {
  namespace esb {
    namespace hive {

      HiveClient::HiveClient(std::string host, int port) {
        _host = host;
        _port = port;
      }

      void HiveClient::connect() {

        logdebug("Connecting to " << _host << " on port " << _port);
        _sock=new org::esb::net::TcpSocket ((char*) _host.c_str(), _port);
        _sock->connect();
        _ois=new org::esb::io::ObjectInputStream(_sock->getInputStream());
        _oos=new org::esb::io::ObjectOutputStream(_sock->getOutputStream());

      }

      void HiveClient::process() {
        int pCount = 0;
        while (true) {
          while (true || ++pCount < 20) {
            char * text = "get process_unit";
            _sock->getOutputStream()->write(text, strlen(text));
            org::esb::hive::job::ProcessUnit unit;
            _ois->readObject(unit);
            if (unit._input_packets.size() == 0)break;
            //		try{
            unit.process();
            //		}catch(...){
            //			logerror("Error in process");
            //		}
            char * text_out = "put process_unit";
            _sock->getOutputStream()->write(text_out, strlen(text_out));
            _oos->writeObject(unit);
            //		break;
          }
          //    break;
          org::esb::lang::Thread::sleep2(1000);
        }
      }

    }
  }
}
