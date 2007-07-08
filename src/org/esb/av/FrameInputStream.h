#include "org/esb/io/InputStream.h"
#include "FrameBaseStream.h"
#include "Frame.h"

using namespace org::esb::io;

namespace org{
    namespace esb{
	    namespace av{
	        class FrameInputStream:public FrameBaseStream, public InputStream{
		    public:
		        FrameInputStream(InputStream * source);
		        ~FrameInputStream();
		        int available(bool isBlocking = false);
		        int read(unsigned char * buffer, int length);
		        int read(vector<unsigned char>&buffer);
		        int read();
		        Frame * readFrame();
		        void close();
		    private:
		    	InputStream * _source;
	        };
	    }
    }
};
