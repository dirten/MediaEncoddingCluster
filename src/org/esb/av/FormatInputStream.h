#ifndef ORG_ESB_AV_FORMATINPUTSTREAM_H
#define ORG_ESB_AV_FORMATINPUTSTREAM_H
#include <boost/thread.hpp>
#include <istream>
#include <vector>
#include <map>

#include "FormatBaseStream.h"
#include "AVInputStream.h"
#include "StreamInfo.h"
#include "TimeStamp.h"


#include "AV.h"
#include "Decoder.h"
#include "exports.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"

using namespace org::esb::io;
namespace org {
  namespace esb {
    namespace av {
      //	    	logger("hive.av")

      class AV_EXPORT FormatInputStream : public FormatBaseStream, public InputStream {
        classlogger("org.esb.av.FormatInputStream")

      public:
        FormatInputStream(File * source);
        FormatInputStream(std::string );
        FormatInputStream(std::istream &str);
        ~FormatInputStream();
        //                DEPRICATED(AVFormatContext * getFormatContext());
        AVFormatContext * getFormatContext();
        long long int available(bool isBlocking = false);
        //DEPRICATED long long int getFileSize();
        bool isValid();
        int read(unsigned char * buffer, int length);
        int read(vector<unsigned char>&buffer);
        int read();
        int getStreamCount();

        /*DEPRICATED*/ InputStream * getStream(int sNumber);
        /*DEPRICATED*/ AVInputStream * getAVStream(int sNumber);
        void close();
        void dumpFormat();
        int seek(int stream_index, int64_t timestamp, int flags = AVSEEK_FLAG_BACKWARD);
        int seek(int stream_index, TimeStamp timestamp, int flags = AVSEEK_FLAG_BACKWARD);
        int selectedStream;
        StreamInfo * getStreamInfo(int idx);
        void init(std::string filename);
        static AVInputFormat * getInputFormat(std::string name);

      private:
        std::string _sourceFile;
        //org::esb::io::File * _file_object;
        bool _isValid;
        //                std::map<int, int> _streamMap;
        //                std::map<int, int> _streamReverseMap;
        friend class PacketInputStream;
        AVFormatContext * formatCtx;
        map<int, StreamInfo*> _stream_info_map;
        static boost::mutex file_open_mutex;
        unsigned char * buffer;
        //int readFunction(void* opaque, uint8_t* buf, int buf_size);
      };
    }
  }
}

#endif

