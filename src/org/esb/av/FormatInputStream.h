#ifndef ORG_ESB_AV_FORMATINPUTSTREAM_H
#define ORG_ESB_AV_FORMATINPUTSTREAM_H
#include "FormatBaseStream.h"
#include "AVInputStream.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/File.h"

#include "avformat.h"
using namespace org::esb::io;
namespace org{
    namespace esb{
	    namespace av{
	        class FormatInputStream: public FormatBaseStream, public InputStream {
		    public:
		        FormatInputStream(File * source);
    		    ~FormatInputStream();
	    	    int available(bool isBlocking = false);
	    	    long getFileSize();
		        int read(unsigned char * buffer, int length);
		        int getStreamCount();
    		    AVInputStream * getStream(int sNumber);
	    	    void close();
		    private:
		        AVFormatContext * formatCtx;
            		org::esb::io::File * _sourceFile;
	        };
    	}
    }
}

#endif

