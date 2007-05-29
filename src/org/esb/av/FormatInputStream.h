#include "FormatBaseStream.h"
#include "AVInputStream.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/File.h"

#include "avformat.h"

namespace org{
    namespace esb{
	    namespace av{
	        class FormatInputStream: public FormatBaseStream, public org::esb::io::InputStream {
		    public:
		        FormatInputStream(org::esb::io::File * source);
    		    ~FormatInputStream();
	    	    int available(bool isBlocking = false);
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
