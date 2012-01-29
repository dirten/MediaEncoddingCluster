/* 
 * File:   FileHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 14. Oktober 2011, 13:17
 */

#include "org/esb/core/PluginContext.h"
#include "FileHandler.h"
#include "plugins/services/webservice/ServiceRequest.h"
#include "plugins/services/webservice/ServiceResponse.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/av/Rational.h"
#include "org/esb/av/FormatInputStream.h"
namespace filehandler {

  FileHandler::FileHandler() {
  }

  FileHandler::~FileHandler() {
  }

  void FileHandler::init() {
    _base = getContext()->getEnvironment<std::string > ("fileapi.baseuri");
  }

  org::esb::core::OptionsDescription FileHandler::getOptionsDescription() {
    org::esb::core::OptionsDescription result("fileapi");
    result.add_options()
            ("fileapi.baseuri", boost::program_options::value<std::string > ()->default_value("/api/v1"), "base uri for the file api requests");
    return result;
  }

  void FileHandler::handleRequest(org::esb::core::Request * req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest * sreq = static_cast<org::esb::api::ServiceRequest *> (req);
    if (sreq->getRequestURI().find(_base+"/file") == 0) {
      handleFile(req, res);
    } else
      if (sreq->getRequestURI().find(_base+"/media") == 0) {
      handleMedia(req, res);
    }
  }

  void FileHandler::handleMedia(org::esb::core::Request * req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest * sreq = static_cast<org::esb::api::ServiceRequest *> (req);
    org::esb::api::ServiceResponse * sres = static_cast<org::esb::api::ServiceResponse *> (res);
    sres->setStatus(org::esb::api::ServiceResponse::OK);
    std::string path = sreq->getRequestURI().substr(13);
    org::esb::io::File infile(path);
    JSONNode result(JSON_NODE);
    if (!infile.exists()) {
      JSONNode error(JSON_NODE);
      error.set_name("error");
      error.push_back(JSONNode("code", "file_not_found"));
      error.push_back(JSONNode("description", std::string("file not found:") + infile.getPath()));
      result.push_back(error);
    } else if (!infile.canRead()) {
      JSONNode error(JSON_NODE);
      error.set_name("error");
      error.push_back(JSONNode("code", "file_not_readable"));
      error.push_back(JSONNode("description", std::string("file could not be read:") + infile.getPath()));
      result.push_back(error);
    } else {
      org::esb::av::FormatInputStream fis(&infile);
      if (!fis.isValid()) {
        JSONNode error(JSON_NODE);
        error.set_name("error");
        error.push_back(JSONNode("code", "file_not_mediafile"));
        error.push_back(JSONNode("description", std::string("file is not a media file:") + infile.getPath()));
        result.push_back(error);
      } else {
        JSONNode data(JSON_NODE);
        data.set_name("data");
        data.push_back(JSONNode("path", infile.getFilePath()));
        data.push_back(JSONNode("name", infile.getFileName()));
        data.push_back(JSONNode("size", fis.getFileSize()));
        if (fis.getFormatContext()) {
          if (fis.getFormatContext()->iformat)
            data.push_back(JSONNode("format", fis.getFormatContext()->iformat->name));
          data.push_back(JSONNode("duration", fis.getFormatContext()->duration));
          data.push_back(JSONNode("bitrate", fis.getFormatContext()->bit_rate));
          int streamcount = fis.getStreamCount();
          for (int a = 0; a < streamcount; a++) {
            org::esb::av::StreamInfo *info = fis.getStreamInfo(a);
            if (info->getCodecType() == AVMEDIA_TYPE_VIDEO || info->getCodecType() == AVMEDIA_TYPE_AUDIO) {
              JSONNode stream(JSON_NODE);
              stream.set_name("stream");
              stream.push_back(JSONNode("codecid", org::esb::av::Decoder::getStaticCodecName(info->getCodec()->codec_id)));
              stream.push_back(JSONNode("type", info->getCodecType() == AVMEDIA_TYPE_AUDIO ? "audio" : "video"));
              stream.push_back(JSONNode("bitrate", info->getCodecBitrate()));
              if (info->getCodecType() == AVMEDIA_TYPE_VIDEO) {
                stream.push_back(JSONNode("timebase", org::esb::av::Rational(info->getCodecTimeBase()).toString()));
                stream.push_back(JSONNode("framerate", org::esb::av::Rational(info->getFrameRate()).toString()));
              }
              if (info->getCodecType() == AVMEDIA_TYPE_AUDIO) {
                stream.push_back(JSONNode("samplerate", info->getCodecSampleRate()));
                stream.push_back(JSONNode("channels", info->getChannels()));
              }
              data.push_back(stream);
            }
          }

        }

        result.push_back(data);

      }
    }
    sres->getOutputStream()->write(result.write_formatted());


  }

  void FileHandler::handleFile(org::esb::core::Request * req, org::esb::core::Response*res) {
    org::esb::api::ServiceRequest * sreq = static_cast<org::esb::api::ServiceRequest *> (req);
    org::esb::api::ServiceResponse * sres = static_cast<org::esb::api::ServiceResponse *> (res);
    JSONNode result(JSON_NODE);
    sres->setStatus(org::esb::api::ServiceResponse::OK);
    std::string path = sreq->getRequestURI().substr(12);
    org::esb::io::File f(path);
    if (f.isDirectory()) {
      org::esb::io::FileList flist = f.listFiles();
      JSONNode array(JSON_ARRAY);
      array.set_name("data");
      foreach(org::esb::io::FileList::value_type &file, flist) {
        buildFile(*file.get(), array);
      }
      result.push_back(array);
    } else
      if (f.isFile()) {
      JSONNode array(JSON_ARRAY);
      array.set_name("data");
      buildFile(f, array);
      result.push_back(array);
    } else {
      result.push_back(JSONNode("error", "file not found"));
    }
    sres->getOutputStream()->write(result.write_formatted());
  }

  void FileHandler::buildFile(org::esb::io::File & file, JSONNode & node) {
    JSONNode data(JSON_NODE);
    data.push_back(JSONNode("path", file.getFilePath()));
    data.push_back(JSONNode("name", file.getFileName()));
    data.push_back(JSONNode("directory", file.isDirectory()));
    node.push_back(data);
  }
}
