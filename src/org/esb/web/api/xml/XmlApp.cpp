/* 
 * File:   XmlApp.cpp
 * Author: HoelscJ
 * 
 * Created on 6. Januar 2011, 15:05
 */

#include "XmlApp.h"
#include "Wt/WResource"
#include "Wt/Http/Request"
#include "org/esb/util/Log.h"
#include "Project.h"
namespace org {
  namespace esb {
    namespace web {

      XmlApp::XmlApp() : Wt::WResource() {
        dataReceived().connect(SLOT(this, XmlApp::getData));
        //setUploadProgress(true);
        
        setRequestTooLarge(10000);

      }

      XmlApp::~XmlApp() {

      }
        void XmlApp::getData(uint64_t a,uint64_t b){
          LOGDEBUG("a:"<<a);
          LOGDEBUG("b:"<<b);
        }

      void XmlApp::handleRequest(const Wt::Http::Request&req, Wt::Http::Response&res) {
        LOGDEBUG("Request Path"<<req.path());
        Wt::Http::ParameterMap map=req.getParameterMap();
        Wt::Http::ParameterMap::iterator mapit=map.begin();
        //Wt::Http::UploadedFileMap map=req.uploadedFiles();
        //Wt::Http::UploadedFileMap::iterator mapit=map.begin();
        for(;mapit!=map.end();mapit++){
          LOGDEBUG("Key="<<(*mapit).first);
        }
        Wt::Http::UploadedFileMap uploads= req.uploadedFiles();
        Wt::Http::UploadedFileMap::iterator upit=uploads.begin();
        for(;upit!=uploads.end();upit++){
          LOGDEBUG("Upload?:"<<upit->second.clientFileName());
          LOGDEBUG("SpoolUpload?:"<<upit->second.spoolFileName());
          upit->second.stealSpoolFile();
        }
        
        //const Wt::Http::ParameterMap map=req.getParameterMap();
        //LOGDEBUG("Upload?:"<<req.uploadedFiles());
        LOGDEBUG("TooLarge?:"<<req.tooLarge());
        LOGDEBUG("DataLength:"<<req.contentLength());
        LOGDEBUG("DataType:"<<req.contentType());
        /*
        char * buffer=new char[req.contentLength()+1];
        memset(buffer,0,req.contentLength()+1);
        req.in().read(buffer, req.contentLength());
        std::string data(buffer,req.contentLength());
        Project prj(data);
        LOGDEBUG(buffer);
        //res.out()<<buffer;
        delete buffer;*/

      }
    }
  }
}
