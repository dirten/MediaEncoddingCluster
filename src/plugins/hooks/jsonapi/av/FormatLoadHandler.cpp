#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/util/Foreach.h"
#include "../JSONResult.h"
#include "../exports.h"

class JSONAPI_EXPORT FormatLoadHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    std::set<std::string> valid_formats;

    valid_formats.insert("amr");
    valid_formats.insert("asf");
    valid_formats.insert("avi");
    valid_formats.insert("avm2");
    valid_formats.insert("dv");
    valid_formats.insert("filmstrip");
    valid_formats.insert("flv");
    valid_formats.insert("gif");
    valid_formats.insert("ipod");
    valid_formats.insert("ivf");
    valid_formats.insert("m4v");
    valid_formats.insert("matroska");
    valid_formats.insert("mjpeg");
    valid_formats.insert("mov");
    valid_formats.insert("mp4");
    valid_formats.insert("mpeg");
    valid_formats.insert("mpegts");
    valid_formats.insert("vcd");
    valid_formats.insert("mpeg1video");
    valid_formats.insert("mpeg2video");
    valid_formats.insert("dvd");
    valid_formats.insert("vob");
    valid_formats.insert("svcd");
    valid_formats.insert("ogg");
    valid_formats.insert("psp");
    valid_formats.insert("rawvideo");
    valid_formats.insert("swf");
    valid_formats.insert("3g2");
    valid_formats.insert("3gp");
    valid_formats.insert("webm");

    
    JSONResult result(req);
    result.push_back(JSONNode("requestUUID", req.get("requestUUID")));
    //result.push_back(JSONNode("format", req.get("formatid")));
    JSONNode c(JSON_NODE);
    c.set_name("data");
    AVOutputFormat *ofmt = NULL;
    bool found=false;
    while ((ofmt = av_oformat_next(ofmt))) {
      if (req.get("formatid")==ofmt->name) {
        c.push_back(JSONNode("longname", ofmt->long_name));
        c.push_back(JSONNode("id", ofmt->name));
        found=true;
      }
    }
    if(found){
      result.push_back(JSONNode("status", "ok"));
      result.push_back(c);      
    }else{
      result.push_back(JSONNode("status", "error"));
      result.push_back(JSONNode("messgage", "Format not found"));
      res.setStatusAndReason(res.HTTP_NOT_FOUND, "Format not found");
    }
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }

};
REGISTER_WEB_HOOK("/api/v1/format/{formatid}$", GET, FormatLoadHandler);





