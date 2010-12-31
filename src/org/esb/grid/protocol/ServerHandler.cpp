/* 
 * File:   ServerHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 22. Dezember 2010, 13:01
 */

#include "ServerHandler.h"
#include "org/esb/util/Log.h"
#include "org/esb/lang/ProcessException.h"
namespace org {
  namespace esb {
    namespace grid {
        Ptr<org::esb::lang::Process> ServerHandler::_ps;
        Ptr<org::esb::lang::Process> ServerHandler::_pc;
        Ptr<org::esb::lang::Process> ServerHandler::_pw;

      ServerHandler::ServerHandler(Ptr<org::esb::net::TcpSocket> s) : NodeCmdHandler(s) {

      }

      bool ServerHandler::handleCommand(std::string & cmddata) {
        LOGDEBUG("handle command: " << cmddata);
        bool result = false;
        if (cmddata.find("start server") != string::npos) {
          result = true;
          LOGDEBUG("starting server")
          std::list<std::string> arguments;
          arguments.push_back("-r");
          _ps = new org::esb::lang::Process("/home/HoelscJ/devel/mec/install/bin/mhive", arguments);
          _ps->run();
        } else
          if (cmddata.find("stop server") != string::npos) {
          result = true;
          LOGDEBUG("stopping server")
          try {
            if (_ps) {
              _ps->stop();
            } else {
              LOGINFO("could not stop the server, because it is not running!")
            }
          } catch (ProcessException & ex) {
            LOGERROR(ex.what())
          }
          try {
            if (_ps && _ps->isRunning()) {
              _ps->kill();
            } else {
              LOGINFO("could not kill the server, because it is not running!")
            }
          } catch (ProcessException & ex) {
            LOGERROR(ex.what())
          }

        }
        return result;
      }

      ServerHandler::~ServerHandler() {
      }
    }
  }
}
