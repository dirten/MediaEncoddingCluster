#include "FormatInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Exception.h"

#include "org/esb/util/StringTokenizer.h"
#include <iostream>
//#include <boost/cast.hpp>
using namespace std;

using namespace org::esb::io;
using namespace org::esb::lang;

using org::esb::util::StringTokenizer;
namespace org {
  namespace esb {
    namespace av {
      boost::mutex FormatInputStream::file_open_mutex;

      static int readFunction(void* opaque, uint8_t* buf, int buf_size) {
        streamsize result=0;
          std::istream & me = *reinterpret_cast<std::istream*>(opaque);
          me.read(reinterpret_cast<char*>(buf), buf_size);
          result=me.gcount();
          //LOGDEBUG("read buffer with size :"<<buf_size<<" bytes readed from stream : "<<result)
          return result;
      }

      FormatInputStream::FormatInputStream(File * source) {
        //_file_object=NULL;
        init(source->getPath());
      }

      FormatInputStream::FormatInputStream(std::string source) {
        //_file_object=NULL;
        init(source);
      }

      FormatInputStream::FormatInputStream(std::istream  & stream){
        const int bufSize= 128 * 1024;
        const int probeBufSize= 2 * 1024;
        unsigned char * buffer = reinterpret_cast<unsigned char*>(av_malloc(bufSize + FF_INPUT_BUFFER_PADDING_SIZE));
        AVIOContext * avioContext= avio_alloc_context(buffer, bufSize, 0, reinterpret_cast<void*>(static_cast<std::istream*>(&stream)), &readFunction, NULL, NULL);

        //AVFormatContext * avFormat = avformat_alloc_context();
        formatCtx = avformat_alloc_context();
        //formatCtx->max_analyze_duration=2147483647;//+=10000000;
        //formatCtx->probesize=2147483647;//+=10000000;
        //auto avFormatPtr = avFormat;
        formatCtx->pb = avioContext;

        /*
        AVProbeData probeData;
        probeData.buf = buffer;
        probeData.buf_size = ulReadBytes;
        probeData.filename = "";
        formatCtx->iformat= av_probe_input_format(&probeData, 1);
        */
        //formatCtx->iformat= getInputFormat("mp4");
        //formatCtx->iformat->flags|=AVFMT_NOFILE;

        formatCtx->flags=AVFMT_FLAG_CUSTOM_IO;
        if (avformat_open_input(&formatCtx, NULL, NULL, NULL) != 0) {
          LOGERROR("could not open stream data");

        }
        LOGINFO("find stream info from stream with analyze duratin of "<<formatCtx->max_analyze_duration<<" and probesize of"<<formatCtx->probesize);
        int search_stream_info=3;
        for(int a=search_stream_info;a>0;a--){
          if (avformat_find_stream_info(formatCtx,NULL) < 0) {
          //if (av_find_stream_info(formatCtx) < 0) {
            LOGERROR("no StreamInfo from stream with anaylze_duration of:"<<formatCtx->max_analyze_duration);
            formatCtx->max_analyze_duration+=1000000;
            //return;
          }else{
            _isValid = true;
            break;
          }
        }
        //avformat_open_input(&avFormat, "dummyFilename", nullptr, nullptr);
      }

      void FormatInputStream::init(std::string source) {
        //boost::mutex::scoped_lock scoped_lock(ffmpeg_mutex);
        LOGINFO("opening InputFile: " << source);
        //_file_object=new org::esb::io::File(source);

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
        LOGDEBUG("FormatInputStream::~FormatInputStream()")
            close();
        //delete _file_object;
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
      /*
      long long int FormatInputStream::getFileSize() {
        return -1;//_file_object->length();//formatCtx->file_size;
      }
  */
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
          avformat_close_input(&formatCtx);
        formatCtx=NULL;
        if (_stream_info_map.size() > 0) {
          map<int, StreamInfo*>::iterator it = _stream_info_map.begin();
          for (; it != _stream_info_map.end(); it++) {
            if((*it).second)
              delete (*it).second;
            //            (*it).second=NULL;
          }
          _stream_info_map.clear();
        }
      }

      AVInputFormat*  FormatInputStream::getInputFormat(std::string name) {
        AVInputFormat *fmt = NULL;
        while ((fmt = av_iformat_next(fmt))) {
          StringTokenizer tok(fmt->name,",");
          while(tok.hasMoreTokens()){
            std::string fmt_name=tok.nextToken();
            if(name==fmt_name){
              return fmt;
            }
          }
        }
        return NULL;
      }

      }
    }
  }


