/* 
 * File:   JsonMediaHandler.cpp
 * Author: HoelscJ
 * 
 * Created on 19. Juli 2011, 15:46
 */
#include "org/esb/hive/DatabaseService.h"

#include "JsonMediaHandler.h"
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
namespace org {
  namespace esb {
    namespace api {

      JsonMediaHandler::JsonMediaHandler() {
      }

      JsonMediaHandler::~JsonMediaHandler() {
      }

      bool JsonMediaHandler::contains(JSONNode& node, std::string name) {
        bool result = false;
        int size = node.size();
        //LOGDEBUG("NodeSize=" << size);
        if (size > 0) {
          for (int a = 0; a < size; a++) {
            JSONNode n = node[a];
            //LOGDEBUG("name=" << n.name());
            if (name == n.name()) {
              result = true;
            }
          }
        }
        return result;
      }

      JSONNode JsonMediaHandler::handle(struct mg_connection *conn, const struct mg_request_info *request_info, db::HiveDb &db, std::string postdata) {
        JSONNode result(JSON_NODE);
        JSONNode inode;
        if (strcmp(request_info->request_method, "GET") == 0) {
          char iddata[1000];
          memset(&iddata, 0, 1000);
          if (request_info->query_string != NULL) {
            mg_get_var(request_info->query_string, strlen(request_info->query_string), "file", iddata, sizeof (iddata));
            LOGDEBUG("DataId" << iddata);
          }

          if (strlen(iddata) > 0) {
            org::esb::io::File infile(iddata);
            if (!infile.exists()) {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "file_not_found"));
              error.push_back(JSONNode("description", "file not found"));
              result.push_back(error);
            } else if (!infile.canRead()) {
              JSONNode error(JSON_NODE);
              error.set_name("error");
              error.push_back(JSONNode("code", "file_not_readable"));
              error.push_back(JSONNode("description", "file could not be read"));
              result.push_back(error);
            } else {
              org::esb::av::FormatInputStream fis(&infile);
              if (!fis.isValid()) {
                JSONNode error(JSON_NODE);
                error.set_name("error");
                error.push_back(JSONNode("code", "file_not_mediafile"));
                error.push_back(JSONNode("description", "file is not a media file"));
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
                      stream.push_back(JSONNode("type", info->getCodecType() == AVMEDIA_TYPE_AUDIO?"audio":"video"));
                      stream.push_back(JSONNode("bitrate", info->getCodecBitrate()));
                      if (info->getCodecType() == AVMEDIA_TYPE_VIDEO){
                        stream.push_back(JSONNode("timebase", org::esb::av::Rational(info->getCodecTimeBase()).toString()));
                        stream.push_back(JSONNode("framerate", org::esb::av::Rational(info->getFrameRate()).toString()));
                      }
                      if (info->getCodecType() == AVMEDIA_TYPE_AUDIO){
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
          } else {
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "no_file"));
            error.push_back(JSONNode("description", "no file name given to retrieve metadata"));
            result.push_back(error);
          }

        } else if (strcmp(request_info->request_method, "POST") == 0) {
          try {
            if (libjson::is_valid(postdata)) {
              LOGDEBUG("Data is valid");
              inode = libjson::parse(postdata);
              if (contains(inode, "file")) {

              } else {
                JSONNode error(JSON_NODE);
                error.set_name("error");
                error.push_back(JSONNode("code", "no_file"));
                error.push_back(JSONNode("description", "no file name given to retrieve metadata"));
                result.push_back(error);
              }
            } else {
              JSONNode error(JSON_NODE);

              error.set_name("error");
              error.push_back(JSONNode("code", "parse_error"));
              error.push_back(JSONNode("description", "no valid json format given"));
              result.push_back(error);

            }
          } catch (std::exception &ex) {
            LOGDEBUG(ex.what());
            JSONNode error(JSON_NODE);
            error.set_name("error");
            error.push_back(JSONNode("code", "parse_error"));
            error.push_back(JSONNode("description", "no valid json format given"));
            result.push_back(error);

          }
        } else {
          JSONNode error(JSON_NODE);
          error.set_name("error");
          error.push_back(JSONNode("code", "method_error"));
          error.push_back(JSONNode("description", "only post and get method is supported"));
          result.push_back(error);
        }
        return result;
      }
    }
  }
}
