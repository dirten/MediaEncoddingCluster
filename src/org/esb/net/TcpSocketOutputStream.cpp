#include "org/esb/io/OutputStream.h"
//#include "TcpSocket.h"
#include "SocketException.h"
#include "org/esb/util/Decimal.h"
#include <errno.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

//#include <exception.h>
using namespace org::esb::io;
using namespace std;
namespace org {
  namespace esb {
    namespace net {
      //boost::mutex thread_write_mutex;

      class TcpSocketOutputStream : public OutputStream {
      private:
        boost::shared_ptr<boost::asio::ip::tcp::socket> _socket;
        boost::mutex & _write_mutex;


      public:

        /******************************************************************************/
        ~TcpSocketOutputStream() {
        }

        /******************************************************************************/
        TcpSocketOutputStream(boost::shared_ptr<boost::asio::ip::tcp::socket> socket, boost::mutex & m) :
        _socket(socket), _write_mutex(m) {

        }

        /******************************************************************************/

        void write(vector<unsigned char>&buffer) {
          write((char*) & buffer[0], buffer.size());
        }

        void write(string&buffer) {
          write((char*) buffer.c_str(), buffer.length());
        }

        /******************************************************************************/
        void write(char * buffer, int len) {
          boost::mutex::scoped_lock lock(_write_mutex);
          if (!_socket->is_open()) {
            _socket->close();
            throw SocketException("SocketOutputStream::write - can not Write, because Socket is not open");
          }

          int remaining = len, byteCounter = 0, bytes = 0;

          /*
           * Send length from buffer
           */
//          try{
          char size[11];
          sprintf((char*) & size, "%010d", len);
          boost::asio::write(*_socket, boost::asio::buffer(&size, 10));
          //          logdebug("sended size: "<<size);
          /*
           * Send buffer
           */
          while (remaining > 0) {
            int sent = boost::asio::write(*_socket, boost::asio::buffer(buffer + (len - remaining), remaining));
            remaining -= sent;
            //            cout << "Data send:"<<sent<<endl;
          }
//          }catch(exception & ex){
//            logerror("Writing to socket:"<<ex.what());
//          }
        }
      };
    }
  }
}



