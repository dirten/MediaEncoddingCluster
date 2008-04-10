#include "WebServer.h"
#include "tnt/tntnet.h"

using namespace org::esb::web;
WebServer::WebServer(){


}

void WebServer::run(){
    tnt::Tntnet app;
    app.listen("0.0.0.0",8008);
    app.mapUrl("^/$","mainpage").setPathInfo("/mainpage");
    app.mapUrl("^/(.*)$","$1");
    app.mapUrl("^/(.*).png$","$1");
    app.run();
}

