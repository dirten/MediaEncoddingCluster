
/* 
 * File:   HTTPServerTest.cpp
 * Author: HoelscJ
 *
 * Created on 24. Januar 2012, 13:10
 */

#include <cstdlib>
#include "org/esb/core/http/Server.h"
#include "org/esb/lang/CtrlCHitWaiter.h"
#include "org/esb/core/http/RequestHandlerFactory.h"
#include "org/esb/core/http/HTTPServerRequest.h"
#include "org/esb/core/http/HTTPRequest.h"
#include "org/esb/core/http/HTTPResponse.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/Environment.h"

using namespace std;
//using namespace Poco::Util;

class RootHandler: public Poco::Net::HTTPRequestHandler
{ 
public: 
void handleRequest(Poco::Net::HTTPServerRequest& request, 
Poco::Net::HTTPServerResponse& response) 
{ 
//Application& app = Application::instance(); 
//app.logger().information("Request from " + request.clientAddress().toString());
  LOGDEBUG("void handleRequest(Poco::Net::HTTPServerRequest& request, ");
response.setChunkedTransferEncoding(true); 
response.setContentType("text/html");

std::ostream& ostr = response.send();
ostr << "<html><head><title>HTTP Server powered by POCO C++ Libraries</title></head>"; 
ostr << "<body>"; 

ostr << "</body></html>"; 
} 
};
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"

class MyRequestHandlerFactory: public
Poco::Net::HTTPRequestHandlerFactory 
{ 
public: 
MyRequestHandlerFactory() 
{
}
Poco::Net::HTTPRequestHandler* createRequestHandler(
const Poco::Net::HTTPServerRequest& request)
{
  LOGDEBUG("Poco::Net::HTTPRequestHandler* createRequestHandler("<<request.getURI());
if (request.getURI() == "/") 
return new RootHandler(); 
else 
return NULL;//new DataHandler(); 
} 
};
/*
 * 
 */
using namespace Poco::Net;
int main2(int argc, char ** argv) {
  Log::open();
  Poco::UInt16 port = 4000;
  HTTPServerParams* pParams = new HTTPServerParams;
  pParams->setMaxQueued(100);
  pParams->setMaxThreads(16);
  ServerSocket svs(port); // set-up a server socket 
  HTTPServer srv(new MyRequestHandlerFactory(), svs, pParams);
  // start the HTTPServer 
  srv.start();
  org::esb::lang::CtrlCHitWaiter::wait();
  //waitForTerminationRequest();
  // Stop the HTTPServer 
  srv.stop();

return 0;
}

class RootHandler2 : public org::esb::core::http::RequestHandler {
public:

  void handle(org::esb::core::http::HTTPServerRequest& request, org::esb::core::http::HTTPServerResponse& response) {
    std::string doc_root=org::esb::config::Config::get("web.docroot");
    LOGDEBUG("Document Root"<<doc_root);
    //org::esb::io::File file()
    LOGDEBUG("handle"<<request.has("testkey"));
    response.setStatus(Poco::Net::HTTPServerResponse::HTTP_NOT_FOUND);
    response.setReason("file not found");
    std::ostream& ostr = response.send();
    return;
    ostr << "<html><head><title>HTTP Server powered by POCO C++ Libraries</title></head>";
    ostr << "<body>";
    ostr << "</body></html>";
  }
};

class HandlerFactory : public org::esb::core::http::RequestHandlerFactory {
public:

  org::esb::core::http::RequestHandler * createHandler(org::esb::core::http::HTTPServerRequest&req) {
    LOGDEBUG("org::esb::core::http::RequestHandler * createHandler(org::esb::core::http::HTTPRequest&req)"<<req.getURI());
    if (req.getMethod() == "POST") {
      LOGDEBUG("POST METHOD");
    }
    if (req.getMethod() == "GET") {
      LOGDEBUG("GET METHOD");
      req.add("testkey","testval");
    }

    return new RootHandler2();
  }
};

int main(int argc, char** argv) {
  org::esb::hive::Environment::build(argc, argv);
  Log::open();
  LOGDEBUG("satrting")
  org::esb::core::http::Server server(4000);

  server.setRequestHandlerFactory(new HandlerFactory());
  server.start();
  org::esb::lang::CtrlCHitWaiter::wait();
  server.stop();
  return 0;
}



