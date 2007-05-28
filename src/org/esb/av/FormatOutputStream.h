#include "FormatBaseStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/File.h"
#include "avformat.h"
namespace org{
    namespace esb{
	namespace io{
	    class FormatOutputStream: public FormatBaseStream, public OutputStream {
		public:
		    FormatOutputStream(File * source);
		    ~FormatOutputStream();
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
