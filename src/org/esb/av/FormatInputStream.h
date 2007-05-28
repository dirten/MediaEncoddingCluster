#include "FormatBaseStream.h"
#include "org/esb/io/InputStream.h"
#include "org/esb/io/File.h"

namespace org{
    namespace esb{
	namespace io{
	    class FormatInputStream: public FormatBaseStream, public InputStream {
		public:
		    FormatInputStream(File * source);
		    ~FormatInputStream();
		    int available(bool isBlocking = false);
		    int read(unsigned char * buffer, int length);
		    void close();
		private:
//		    static void initialize();
	    };
	}
    }
};
