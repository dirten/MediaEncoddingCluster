/* 
 * File:   MHiveServerConnection.cpp
 * Author: HoelscJ
 * 
 * Created on 4. April 2011, 16:54
 */

#include "MHiveServerConnection.h"

#include "org/esb/net/TcpSocket.h"
namespace org {
  namespace esb {
    namespace api {

      MHiveServerConnection::MHiveServerConnection(std::string host,std::string user, std::string passwd) : _host(host), _name(user), _passwd(passwd), _socket(new org::esb::net::TcpSocket(host, 6000)), _ctrl(new org::esb::rpc::Controller()) {
      }

      void MHiveServerConnection::connect() {
        try {
          _socket->connect();
        } catch (std::exception & ex) {
          std::cout << ex.what() << std::endl;
        }
        ch.reset(new org::esb::rpc::Channel(_socket.get()));
        _api.reset(new org::esb::rpc::ApiService::Stub((ch.get())));
        org::esb::rpc::User user;
        org::esb::rpc::AuthToken token;
        user.set_name("jh");
        user.set_pass("jh");
        _api->authenticate(_ctrl.get(),&user,&token,NULL);
        if (_ctrl->Failed()) {
          LOGERROR("Service request failed:" << _ctrl->ErrorText());
        } else {
          _token=token.token();
        }
      }

      void MHiveServerConnection::disconnect() {
        _socket->close();
      }

      std::list<org::esb::rpc::Mediafile> MHiveServerConnection::getMediaFiles() {
        std::list<org::esb::rpc::Mediafile>result;
        org::esb::rpc::MediafileRequest req;
        org::esb::rpc::MediafileResponse res;
        _api->getMediafileList(_ctrl.get(), &req, &res, NULL);

        if (_ctrl->Failed()) {
          LOGERROR("Service request failed:" << _ctrl->ErrorText());
        } else {
          for (int i = 0; i < res.mediafiles_size(); i++) {
            result.push_back(org::esb::rpc::Mediafile(res.mediafiles(i)));
          }
        }

        return result;
      }

      MHiveServerConnection::~MHiveServerConnection() {
        disconnect();
      }
    }
  }
}
