/* 
 * File:   FileHandler.h
 * Author: HoelscJ
 *
 * Created on 14. Oktober 2011, 13:17
 */

#ifndef FILEHANDLER_H
#define	FILEHANDLER_H
#include "org/esb/core/HookPlugin.h"
#include "org/esb/io/File.h"
#include "org/esb/libjson/JSONNode.h"

namespace filehandler {

  class FileHandler: public org::esb::core::HookPlugin {
  public:
    FileHandler();
    virtual ~FileHandler();
    void handleRequest(org::esb::core::Request * req, org::esb::core::Response*res);
    org::esb::core::OptionsDescription getOptionsDescription();
  private:
    void handleFile(org::esb::core::Request * req, org::esb::core::Response*res);
    void handleMedia(org::esb::core::Request * req, org::esb::core::Response*res);
    void buildFile(org::esb::io::File &, JSONNode & node);
  };
  REGISTER_HOOK("web.api.Service",FileHandler, FileHandler::handleRequest,1);
}

#endif	/* FILEHANDLER_H */

