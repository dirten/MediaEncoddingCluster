#include "InputStream.h"
#include "File.h"

namespace org{
    namespace esb{
	namespace io{
	    class FormatInputStream: public InputStream{
		public:
		    FormatInputStream(File * source);
		    ~FormatInputStream();
		    int available(bool isBlocking = false);
		    int read(unsigned char * buffer, int length);
		    void close();
		private:
		    static void initialize();
	    };
	}
    }
};
