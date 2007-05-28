#include "org/esb/io/InputStream.h"
#include "org/esb/io/File.h"

namespace org{
    namespace esb{
	namespace io{
	    class FrameInputStream:public InputStream{
		public:
		    FrameInputStream(File * source);
		    ~FrameInputStream();
		    int available(bool isBlocking = false);
		    int read(unsigned char * buffer, int length);
		    void close();
	    };
	}
    }
};
