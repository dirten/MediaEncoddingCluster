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
      boost::mutex FormatInputStream::file_open_mutex;

      FormatInputStream::FormatInputStream(File * source) {
        init(source->getPath());
      }

      FormatInputStream::FormatInputStream(std::string source) {
        init(source);
      }
      
      void FormatInputStream::init(std::string source) {
	//boost::mutex::scoped_lock scoped_lock(ffmpeg_mutex);
        LOGINFO("opening InputFile: " << source);
        _isValid = false;
        _sourceFile = source;

        formatCtx = avformat_alloc_context();
//        formatCtx->flags |= AVFMT_FLAG_GENPTS;
//        formatCtx->flags |= AVFMT_FLAG_NONBLOCK;
        //        AVInputFormat*iformat = av_find_input_format("mpegts");
        //formatCtx->debug=5;
        //std::string filename = _sourceFile->getPath();
		{
	        

        if (avformat_open_input(&formatCtx, _sourceFile.c_str(), NULL, NULL) != 0) {
          LOGERROR("could not open " << _sourceFile);
          return;
        }
		}

        LOGINFO("find stream info: " << source);
        if (avformat_find_stream_info(formatCtx,NULL) < 0) {
          LOGERROR("no StreamInfo from:" << _sourceFile);
          return;
        }
        if (formatCtx->iformat->flags & AVFMT_TS_DISCONT) {
          LOGDEBUG("TS DISCONT");
        }
        /**
         * get the first video and audio stream
         */
        //		  loginfo("create stream map: "<<source->getPath());
        /*
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
                }*/
        _isValid = true;
        LOGDEBUG("file openned: " << source);

      }

      bool FormatInputStream::isValid() {
        return _isValid;
      }

      AVFormatContext * FormatInputStream::getFormatContext() {
        return formatCtx;
      }

      void FormatInputStream::dumpFormat() {
        av_dump_format(formatCtx, 0, _sourceFile.c_str(), false);
      }

      FormatInputStream::~FormatInputStream() {
        close();
      }

      int FormatInputStream::getStreamCount() {
        return formatCtx->nb_streams;
      }

      InputStream * FormatInputStream::getStream(int streamIndex) {
        selectedStream = streamIndex;
        return this;
      }

      StreamInfo * FormatInputStream::getStreamInfo(int idx) {
        if (_stream_info_map.find(idx) == _stream_info_map.end()) {
          _stream_info_map[idx] = new StreamInfo(formatCtx->streams[idx], idx);
        }
        return _stream_info_map[idx];
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

      int FormatInputStream::seek(int stream_index, int64_t timestamp, int flags) {
        return av_seek_frame(formatCtx, stream_index, timestamp,flags);
      }

      int FormatInputStream::seek(int stream_index, TimeStamp timestamp, int flags) {
        return seek(stream_index, timestamp.getTime(),flags);
      }

      /*closing the input file and delete all StreamInfo for that file*/
      void FormatInputStream::close() {
        if (_isValid&&formatCtx)
          av_close_input_file(formatCtx);
        formatCtx=NULL;
        if (_stream_info_map.size() > 0) {
          map<int, StreamInfo*>::iterator it = _stream_info_map.begin();
          for (; it != _stream_info_map.end(); it++) {
//            if((*it).second)
//              delete (*it).second;
//            (*it).second=NULL;
          }
          _stream_info_map.clear();
        }
      }
    }
  }
}


