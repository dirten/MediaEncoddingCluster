#include "org/esb/io/InputStream.h"
//#include "org/esb/net/TcpSocket.h"
#include "SocketException.h"
#include <iostream>
//#include "../config.h"
#include <vector>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "org/esb/util/Log.h"
using boost::asio::ip::tcp;

using namespace org::esb::lang;
using namespace org::esb::io;
//using namespace org::esb::net;
using namespace std;

//using namespace org::esb::net;
namespace org {
  namespace esb {
    namespace net {

      class TcpSocketInputStream : public InputStream {
      private:
        boost::system::error_code error;
        boost::shared_ptr<tcp::socket> _socket;
        boost::mutex & _read_mutex;
        char byte;

      public:

        /******************************************************************************/
        ~TcpSocketInputStream() {
        }

        /******************************************************************************/

        TcpSocketInputStream(boost::shared_ptr<tcp::socket> socket, boost::mutex & m) :
        _socket(socket), _read_mutex(m) {

        }

        /******************************************************************************/

        int read(vector<unsigned char>&buffer) {
          size_t size = buffer.size();
          buffer.clear();
          unsigned char * tmp_buffer = new unsigned char[size];
          size_t counter = read(tmp_buffer, size);
          for (std::size_t ix = 0; ix < counter; ++ix) {
            buffer.push_back(tmp_buffer[ix]);
          }
          delete tmp_buffer;
          return counter;
        }

        /******************************************************************************/
        int read() {
          read((unsigned char*) & byte, 1);
          return byte;
        }

        /******************************************************************************/

        int read(unsigned char * buffer, int length) {
          boost::mutex::scoped_lock lock(_read_mutex);
          if (!_socket->is_open()) {
            _socket->close();
            throw SocketException("SocketOutputStream::read - can not Read, because Socket is not open");
          }
          int counter = 0, remaining = length;

          //          while (remaining > 0) {
          int read = boost::asio::read(*_socket, boost::asio::buffer(buffer, length), boost::asio::transfer_at_least(length), error);
          //            int read = _socket->read_some(boost::asio::buffer(buffer + (length - remaining), remaining), error);
          //            remaining -= read;
          //            counter += read;
          LOGTRACE("org.esb.net.TcpSocketInputStream", "readed bytes:" << read);
          //          }

          if (error)
            throw boost::system::system_error(error);
          return read;
        }

        int read(string & str) {
          /*Receive length of data*/
          int length = static_cast<int> (available(true));
          LOGTRACE("org.esb.net.TcpSocketInputStream", "size to read:" << length);
          unsigned char * buffer = new unsigned char[length];
          int counter = 0;
          /*Receive data into buffer*/
          counter = read(buffer, length);
          LOGTRACE("org.esb.net.TcpSocketInputStream", "size readed:" << counter);
          /*If Connection is dead*/
          if (counter <= 0) {
            LOGERROR("org.esb.net.TcpSocketInputStream", "Socket is brocken");
          } else {
            str = string((char*) buffer, length);
          }
          delete []buffer;
          return counter;
        }

        /******************************************************************************/

        /*Receive length of buffer*/
        long long int available(bool isBlocking) {
          char tmp[11];
          memset(&tmp, 0, sizeof (tmp));
          //          int len = 0;
          int read = boost::asio::read(*_socket, boost::asio::buffer(&tmp, 10), boost::asio::transfer_at_least(10));
          //          _socket->read_some(boost::asio::buffer(&tmp, 10), error);
          if (error)
            throw boost::system::system_error(error);
          else if (read != 10)
            throw SocketException("reading size is not equal 10");
          //            return 0;
          //			else
          //				throw boost::system::system_error(error);
          return atoi(tmp);
        }
      };
    }
  }
}

