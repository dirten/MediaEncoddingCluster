#ifndef ORG_ESB_AV_FORMATINPUTSTREAM_H
#define ORG_ESB_AV_FORMATINPUTSTREAM_H
#include "FormatBaseStream.h"
#include "AVInputStream.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/util/Log.h"
#include <vector>


#include "AV.h"


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
                AVFormatContext * getFormatContext();
                long long int available(bool isBlocking = false);
                long long int getFileSize();
                int read(unsigned char * buffer, int length);
                int read(vector<unsigned char>&buffer);
                int read();
                int getStreamCount();
//                __attribute__((deprecated))InputStream * getStream(int sNumber);
                InputStream * getStream(int sNumber);
                AVInputStream * getAVStream(int sNumber);
                void close();
                void dumpFormat();
                int seek(int stream_index, int64_t timestamp);
                int selectedStream;
            private:
                org::esb::io::File * _sourceFile;
            protected:
                AVFormatContext * formatCtx;
            };
        }
    }
}

#endif

