#include "FormatBaseStream.h"
#include "org/esb/io/OutputStream.h"
#include "org/esb/io/File.h"
extern "C" {
#include "avformat.h"
}
namespace org{
    namespace esb{
	namespace av{
	    class FormatOutputStream: public FormatBaseStream, public org::esb::io::OutputStream {
		public:
		    FormatOutputStream(org::esb::io::File * source);
		    ~FormatOutputStream();
		    int write(unsigned char * buffer, int length);
		    void close();
		private:

	    };
	}
    }
};
