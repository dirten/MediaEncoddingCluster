/* 
 * File:   RootRequestHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 3. Februar 2012, 15:58
 */

#include "RootRequestHandler.h"
#include "org/esb/util/Log.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/config/config.h"
namespace org {
  namespace esb {
    namespace core {
      namespace http {

        RootRequestHandler::RootRequestHandler() {
        }

        RootRequestHandler::~RootRequestHandler() {
        }
        void RootRequestHandler::handle(HTTPServerRequest& request, HTTPServerResponse& response){
          std::string doc_root=org::esb::config::Config::get("web.docroot");
          LOGDEBUG("DocRoot:"<<doc_root);
          std::string uri=request.getURI();
          LOGDEBUG("URI"<<uri);
          if(uri=="/")
            uri+="index.html";
          org::esb::io::File file(doc_root+uri);
          LOGDEBUG("RequestFile:"<<file.getPath());
          std::string data;
          if(file.getPath().find(doc_root)!= string::npos){
            LOGDEBUG("Path is in the Docroot");
            if(file.exists()){
              org::esb::io::FileInputStream fis(&file);
              fis.read(data);
              LOGDEBUG("Jspos:"<<file.getExtension());
              if(file.getExtension().find("js")!= string::npos){
                response.setContentType("application/javascript");
              }else if(file.getExtension().find("png")!= string::npos){
                response.setContentType("image/png");
              }else if(file.getExtension().find("html")!= string::npos){
                response.setContentType("text/html");
              }else{
                response.setContentType("text/plain");      
              }
            }else{
              response.setStatusAndReason(response.HTTP_NOT_FOUND,"Resource not found");
            }
          }else{
            response.setStatusAndReason(response.HTTP_NOT_FOUND,"Resource not found");
            LOGDEBUG("Path "<<file.getPath()<<"is not in the Docroot"<<doc_root);
          }
          //response.setContentType("application/json");
          std::ostream& ostr = response.send();
          ostr << data;
        }
      }
    }
  }
}
