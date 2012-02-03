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
          LOGDEBUG("URI"<<request.getURI());
          org::esb::io::File file(doc_root+request.getURI());
          LOGDEBUG("RequestFile:"<<file.getPath());
          std::string data;
          if(file.getPath().find(doc_root)!= string::npos){
            LOGDEBUG("Path is in the Docroot");
            if(file.exists()){
              org::esb::io::FileInputStream fis(&file);
              fis.read(data);
              if(file.getPath().find("js")!= string::npos){
                response.setContentType("text/plain");
              }
            }else{
              response.setStatusAndReason(response.HTTP_NOT_FOUND,"Resource not found");
            }
          }else{
            LOGDEBUG("Path is not in the Docroot");
          }
          response.setContentType("text/html");
          //response.setContentType("application/json");
          std::ostream& ostr = response.send();
          ostr << data;
        }
      }
    }
  }
}
