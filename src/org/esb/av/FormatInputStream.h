#ifndef ORG_ESB_AV_FORMATINPUTSTREAM_H
#define ORG_ESB_AV_FORMATINPUTSTREAM_H
#include "FormatBaseStream.h"
#include "AVInputStream.h"
#include "StreamInfo.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"

#include <vector>
#include <map>

#include "AV.h"

#ifdef WIN32
#define DEPRICATED(func) __declspec(deprecated) func
#else
#define DEPRICATED(f) __attribute__((deprecated)) f
#endif
//extern "C" AVStream;
using namespace org::esb::io;
namespace org {
    namespace esb {
        namespace av {
            //	    	logger("hive.av")

            class FormatInputStream : public FormatBaseStream, public InputStream {
                logger("hive.av.formatis");

            public:
                FormatInputStream(File * source);
                ~FormatInputStream();
                DEPRICATED(AVFormatContext * getFormatContext());
                long long int available(bool isBlocking = false);
                long long int getFileSize();
                bool isValid();
                int read(unsigned char * buffer, int length);
                int read(vector<unsigned char>&buffer);
                int read();
                int getStreamCount();
                //                __attribute__((deprecated))InputStream * getStream(int sNumber);
               DEPRICATED(InputStream * getStream(int sNumber));
               DEPRICATED(AVInputStream * getAVStream(int sNumber));
                void close();
                void dumpFormat();
                int seek(int stream_index, int64_t timestamp);
                int selectedStream;
                StreamInfo * getStreamInfo(int idx);
            private:
                org::esb::io::File * _sourceFile;
                bool _isValid;
                std::map<int, int> _streamMap;
                std::map<int, int> _streamReverseMap;
                friend class PacketInputStream;
                AVFormatContext * formatCtx;

            };
        }
    }
}

#endif

