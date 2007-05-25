#ifndef ORG_ESB_IO_FILEINPUTSTREAM_H
#define ORG_ESB_IO_FILEINPUTSTREAM_H
#include "InputStream.h"
#include "File.h"
#include "org/esb/lang/Exception.h"

namespace org{
    namespace esb{
	namespace io{
	    class FileInputStream : public InputStream{
		public:
		    FileInputStream(File * file)throw (org::esb::lang::Exception);
		    int available(bool i);
		    int read(unsigned char * buffer, int length);
		    void close();
		private:
		    void open(const char * name);
	    };
	}
    }
}
#endif
