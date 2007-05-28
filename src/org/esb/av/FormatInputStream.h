#include "FormatBaseStream.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/File.h"
#include "avformat.h"

namespace org{
    namespace esb{
	namespace io{
	    class FormatInputStream: public FormatBaseStream, public InputStream {
		public:
		    FormatInputStream(File * source);
		    ~FormatInputStream();
		    int available(bool isBlocking = false);
		    int read(unsigned char * buffer, int length);
		    int getStreamCount();
		    int getStream(int sNumber);
		    void close();
		private:
		    AVFormatContext formatCtx;
//		    static void initialize();
	    };
	}
    }
};
