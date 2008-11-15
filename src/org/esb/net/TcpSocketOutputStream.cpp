#include "org/esb/io/OutputStream.h"
#include "TcpSocket.h"
#include "SocketException.h"
#include "org/esb/util/Decimal.h"
#include <errno.h>

//#include <exception.h>
using namespace org::esb::io;
using namespace std;
namespace org {
  namespace esb {
    namespace net {

      class TcpSocketOutputStream : public OutputStream {
      private:
        boost::shared_ptr<tcp::socket> _socket;

      public:

                /******************************************************************************/
        ~TcpSocketOutputStream() {
        }

                /******************************************************************************/
        TcpSocketOutputStream(boost::shared_ptr<tcp::socket> socket) : _socket(socket) {

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

          if (!_socket->is_open()) {
            throw SocketException("SocketOutputStream::write - can not Write, because Socket is allready Closed");
          }

          int remaining = len, byteCounter = 0, bytes = 0;

          /*
           * Send length from buffer
           */

          char size[11];
          sprintf((char*)&size, "%010d", len);
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
        }
      };
    }
  }
}



