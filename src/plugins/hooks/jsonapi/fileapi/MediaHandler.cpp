#include "org/esb/core/WebHookPlugin.h"
#include "org/esb/libjson/libjson.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Foreach.h"
#include "org/esb/av/FormatInputStream.h"
#include "../JSONResult.h"
#include "../exports.h"

class JSONAPI_EXPORT MediaHandler : public org::esb::core::WebHookPlugin {
public:

  void handle(org::esb::core::http::HTTPServerRequest&req, org::esb::core::http::HTTPServerResponse&res) {
    JSONResult result(req);
    if (req.has("path")) {
      std::string path = req.get("path");
      //result.push_back(JSONNode("file", req.get("path")));
      org::esb::io::File infile(path);
      if (!infile.exists()) {
        result.setStatus(res.HTTP_NOT_FOUND,"file_not_found",std::string("file not found:") + infile.getPath());
      } else if (!infile.canRead()) {
        result.setStatus(res.HTTP_NOT_FOUND,"file_not_found",std::string("file could not be read:") + infile.getPath());
      } else {
        org::esb::av::FormatInputStream fis(&infile);
        if (!fis.isValid()) {
        result.setStatus(res.HTTP_BAD_REQUEST,"file_not_found",std::string("file is not a media file:") + infile.getPath());
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
            JSONNode streams(JSON_ARRAY);
            streams.set_name("streams");
            for (int a = 0; a < streamcount; a++) {
              org::esb::av::StreamInfo *info = fis.getStreamInfo(a);
              if (info->getCodecType() == AVMEDIA_TYPE_VIDEO || info->getCodecType() == AVMEDIA_TYPE_AUDIO) {
                JSONNode stream(JSON_NODE);
                stream.set_name("stream");
                stream.push_back(JSONNode("codecid", org::esb::av::Decoder::getStaticCodecName(info->getCodec()->codec_id)));
                stream.push_back(JSONNode("type", info->getCodecType() == AVMEDIA_TYPE_AUDIO ? "audio" : "video"));
                stream.push_back(JSONNode("stream_index", info->getIndex()));
                stream.push_back(JSONNode("bitrate", info->getCodecBitrate()));
                if (info->getCodecType() == AVMEDIA_TYPE_VIDEO) {
                  stream.push_back(JSONNode("timebase", org::esb::av::Rational(info->getCodecTimeBase()).toString()));
                  stream.push_back(JSONNode("framerate", org::esb::av::Rational(info->getFrameRate()).toString()));
                }
                if (info->getCodecType() == AVMEDIA_TYPE_AUDIO) {
                  stream.push_back(JSONNode("samplerate", info->getCodecSampleRate()));
                  stream.push_back(JSONNode("channels", info->getChannels()));
                }
                streams.push_back(stream);
              }
            }
            data.push_back(streams);
          }
          result.push_back(data);
        }
      }
    }
    res.setContentType("text/plain");
    std::ostream& ostr = res.send();
    ostr << result.write_formatted();
  }
};
REGISTER_WEB_HOOK("/api/v1/media/?{path}", GET, MediaHandler);




