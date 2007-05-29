#include "FormatBaseStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/File.h"
#include "avformat.h"
namespace org{
    namespace esb{
	namespace av{
	    class FormatOutputStream: public FormatBaseStream, public org::esb::io::OutputStream {
		public:
		    FormatOutputStream(org::esb::io::File * source);
		    ~FormatOutputStream();
//		    int available(bool isBlocking = false);
		    int write(unsigned char * buffer, int length);
//		    int getStreamCount();
//		    int getStream(int sNumber);
		    void close();
		private:
		    AVFormatContext formatCtx;
//		    static void initialize();
	    };
	}
    }
};
