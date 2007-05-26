#include "InputStream.h"
#include "File.h"

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
