#include "org/esb/io/InputStream.h"
#include "FrameBaseStream.h"
//#include "org/esb/io/File.h"
using namespace org::esb::io;

namespace org{
    namespace esb{
	    namespace io{
	        class FrameInputStream:public FrameBaseStream, public InputStream{
		    public:
		        FrameInputStream(InputStream * source);
		        ~FrameInputStream();
		        int available(bool isBlocking = false);
		        int read(unsigned char * buffer, int length);
		        void close();
	        };
	    }
    }
};
