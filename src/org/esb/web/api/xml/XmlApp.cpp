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
#include "org/esb/hive/xml/JobCreator.h"
#include "org/esb/io/File.h"
#include "org/esb/io/FileInputStream.h"
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

      void XmlApp::getData(uint64_t a, uint64_t b) {
        LOGDEBUG("a:" << a);
        LOGDEBUG("b:" << b);
      }

      void XmlApp::handleRequest(const Wt::Http::Request&req, Wt::Http::Response&res) {
        LOGDEBUG("Request Path" << req.pathInfo());
        if(req.method()!="GET"){
          res.setStatus(200);
        }else
          res.setStatus(404);
        
        Wt::Http::ParameterMap map = req.getParameterMap();
        Wt::Http::ParameterMap::iterator mapit = map.begin();
        //Wt::Http::UploadedFileMap map=req.uploadedFiles();
        //Wt::Http::UploadedFileMap::iterator mapit=map.begin();
        if (map.count("xml")) {
          std::string xml = *req.getParameter("xml");
          //std::string * xml=(*map.find("xml")).second;
          org::esb::hive::JobCreator cr;
          cr.setXml(xml);
        }
        for (; mapit != map.end(); mapit++) {
          LOGDEBUG("Key=" << (*mapit).first);
          LOGDEBUG("value="<<*req.getParameter((*mapit).first));
        }
        Wt::Http::UploadedFileMap uploads = req.uploadedFiles();
        Wt::Http::UploadedFileMap::iterator upit = uploads.begin();
        for (; upit != uploads.end(); upit++) {
          //std::string xml=*req.getParameter("xml");

          org::esb::io::File file(upit->second.spoolFileName());
          if (!file.exists()) {
            LOGERROR("Preset File does not exist! " << file.getPath());
            return;
          }
          //LOGINFO("reading presets from "<<filename);
          org::esb::io::FileInputStream fis(&file);
          std::string data;
          fis.read(data);

          org::esb::hive::JobCreator cr;
          cr.setXml(data);
          LOGDEBUG("Upload?:" << upit->second.clientFileName());
          LOGDEBUG("SpoolUpload?:" << upit->second.spoolFileName());
          upit->second.stealSpoolFile();
          res.setStatus(200);

        }

        //const Wt::Http::ParameterMap map=req.getParameterMap();
        //LOGDEBUG("Upload?:"<<req.uploadedFiles());
        LOGDEBUG("TooLarge?:" << req.tooLarge());
        LOGDEBUG("DataLength:" << req.contentLength());
        LOGDEBUG("DataType:" << req.contentType());
        /*
        char * buffer=new char[req.contentLength()+1];
        memset(buffer,0,req.contentLength()+1);
        req.in().read(buffer, req.contentLength());
        std::string data(buffer,req.contentLength());
        Project prj(data);
        LOGDEBUG(buffer);
        res.out()<<buffer;
        delete buffer;*/
        res.out()<<"job created";

      }
    }
  }
}
