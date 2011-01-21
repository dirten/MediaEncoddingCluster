/* 
 * File:   Client.h
 * Author: HoelscJ
 *
 * Created on 12. Januar 2011, 20:18
 */

#ifndef CLIENT_H
#define	CLIENT_H
#include <string>
namespace org {
  namespace esb {
    namespace rpc {

      class Client {
      public:
        Client(std::string host, int port);
        virtual ~Client();
      private:

      };
    }
  }
}

#endif	/* CLIENT_H */

