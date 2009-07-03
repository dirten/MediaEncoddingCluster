#include "FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Exception.h"

#include <iostream>
//#include <boost/cast.hpp>
using namespace std;

using namespace org::esb::io;
using namespace org::esb::lang;
namespace org {
  namespace esb {
    namespace av {

      FormatInputStream::FormatInputStream(File * source) {
		  loginfo("opening InputFile: "<<source->getPath());
		  _isValid = false;
        _sourceFile = source;
        formatCtx = avformat_alloc_context();
        if (av_open_input_file(&formatCtx, _sourceFile->getPath().c_str(), NULL, 0, NULL) != 0) {
          logerror("Konnte Datei " << _sourceFile->getPath() << " nicht oeffnen");
          return;
        }
		  loginfo("find stream info: "<<source->getPath());
        if (av_find_stream_info(formatCtx) < 0) {
          logerror("Konnte StreamInfo von " << _sourceFile->getPath() << " nicht ermitteln");
          return;
        }
        /**
         * get the first video and audio stream
         */
		  loginfo("create stream map: "<<source->getPath());
        _streamMap[0] = -1;
        _streamMap[1] = -1;
        for (int i = 0; i < formatCtx->nb_streams; i++) {
          if (formatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO && _streamMap[0] == -1) {
            _streamMap[0] = i;
            _streamReverseMap[i] = 0;
          } else
            if (formatCtx->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO && _streamMap[1] == -1) {
            _streamMap[1] = i;
            _streamReverseMap[i] = 1;
          } else
            _streamReverseMap[i] = -1;
        }
        if(_streamMap[0]<0){
            _streamMap[0] = _streamMap[1];
            _streamReverseMap[_streamMap[1]] = 1;
        }
        _isValid = true;
		  loginfo("file openned: "<<source->getPath());

	  }

      bool FormatInputStream::isValid() {
        return _isValid;
      }

      AVFormatContext * FormatInputStream::getFormatContext() {
        return formatCtx;
      }

      void FormatInputStream::dumpFormat() {
        dump_format(formatCtx, 0, _sourceFile->getPath().c_str(), false);
      }

      FormatInputStream::~FormatInputStream() {
        close();
      }

      int FormatInputStream::getStreamCount() {
        return _streamMap.size();
      }

      InputStream * FormatInputStream::getStream(int streamIndex) {
        selectedStream = streamIndex;
        return this;
      }

      StreamInfo * FormatInputStream::getStreamInfo(int idx) {
        return new StreamInfo(formatCtx->streams[_streamMap[idx]], idx);
      }

      AVInputStream * FormatInputStream::getAVStream(int streamIndex) {
        //		AVInputStream * str=(AVInputStream*)formatCtx->streams[streamIndex];
        return (AVInputStream*) formatCtx->streams[streamIndex];
      }

      long long int FormatInputStream::available(bool withBlocking) {
        return 0;
      }

      long long int FormatInputStream::getFileSize() {
        return formatCtx->file_size;
      }

      int FormatInputStream::read() {
        return 0;
      }

      int FormatInputStream::read(unsigned char * buffer, int length) {
        return 0;
      }

      int FormatInputStream::read(vector<unsigned char>&buffer) {
        return 0;
      }

      int FormatInputStream::seek(int stream_index, int64_t timestamp) {
        return av_seek_frame(formatCtx, stream_index, timestamp, AVSEEK_FLAG_ANY);
        //return av_seek_frame(formatCtx, stream_index, timestamp,AVSEEK_FLAG_BACKWARD	);
      }

      void FormatInputStream::close() {
        if (formatCtx)
          av_close_input_file(formatCtx);
      }
    }
  }
}


