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
		    FormatOutputStream(org::esb::io::File * target);
		    ~FormatOutputStream();
		    void write(unsigned char * buffer, int length);
		    void write(vector<unsigned char>&buffer);
		    void write(unsigned char * buffer);
		    void close();
		    void addPacketStream(PacketOutputStream & stream);
		private:

	    };
	}
    }
};
