#include "org/esb/io/OutputStream.h"

#include <errno.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "SocketException.h"

//#include <exception.h>
using namespace org::esb::io;
using namespace std;
namespace org {
  namespace esb {
    namespace net {
      //boost::mutex thread_write_mutex;

      class TcpSocketOutputStream : public OutputStream {
        classlogger("org.esb.net.TcpSocketOututStream")

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

        void writeRaw(string&buffer) {
          /*
           * Send buffer
           */
          boost::system::error_code error;
          int sent = boost::asio::write(*_socket, boost::asio::buffer(buffer, buffer.length()), boost::asio::transfer_all(), error);
          if (error)
            throw boost::system::system_error(error);
          else if (sent != buffer.length())
            throw SocketException("writing size is not equal to len");

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
          boost::system::error_code error;
          /*
           * Send length from buffer
           */
          //          try{
          char size[11];
          sprintf((char*) & size, "%010d", len);
          boost::asio::write(*_socket, boost::asio::buffer(&size, 10));
          //LOGDEBUG("sended size: " << size);
          /*
           * Send buffer
           */
          int sent = boost::asio::write(*_socket, boost::asio::buffer(buffer, len), boost::asio::transfer_all(), error);
          if (error)
            throw SocketException(std::string("while writing on socket:").append(error.message()));
            //throw boost::system::system_error(error.);
          else if (sent != len)
            throw SocketException("writing size is not equal to len");
          //            remaining -= sent;
          //            cout << "Data send:"<<sent<<endl;
          //          }
          //          }catch(exception & ex){
          //            logerror("Writing to socket:"<<ex.what());
          //          }
        }
      };
    }
  }
}



