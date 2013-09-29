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

#include "Poco/StringTokenizer.h"
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
          org::esb::io::File dr_file(doc_root);
          LOGDEBUG("DocRoot:"<<dr_file.getPath());
          std::string uri=request.getURI();
          Poco::StringTokenizer tokenz(uri,"?");//parsing out the get parameter
          uri=tokenz[0];
          LOGDEBUG("URI"<<uri);
          if(uri=="/")
            uri+="index.html";
          org::esb::io::File file(dr_file.getPath()+uri);
          LOGDEBUG("RequestFile:"<<file.getPath());
          std::string data;
          if(file.getPath().find(dr_file.getPath())!= string::npos){
            LOGDEBUG("Path is in the Docroot");
            if(file.exists()){
              org::esb::io::FileInputStream fis(&file);
              fis.read(data);
              LOGDEBUG("Jspos:"<<file.getExtension());
              if(file.getExtension().find("js")!= string::npos){
                response.setContentType("application/javascript; charset=UTF-8");
              }else if(file.getExtension().find("png")!= string::npos){
                response.setContentType("image/png");
              }else if(file.getExtension().find("html")!= string::npos){
                response.setContentType("text/html; charset=UTF-8");
              }else if(file.getExtension().find("sj")!= string::npos){
                response.setContentType("application/javascript; charset=UTF-8");
              }else if(file.getExtension().find("css")!= string::npos){
                response.setContentType("text/css; charset=UTF-8");
              }else{
                response.setContentType("text/plain; charset=UTF-8");
              }
            }else{
              response.setStatusAndReason(response.HTTP_NOT_FOUND,"Resource not found");
            }
          }else{
            response.setStatusAndReason(response.HTTP_NOT_FOUND,"Resource not found");
            LOGDEBUG("Path "<<file.getPath()<<" is not in the Docroot "<<dr_file.getPath());
          }
          //response.setContentType("application/json");
          std::ostream& ostr = response.send();
          ostr << data;
        }
        }
      }
    }
  }
