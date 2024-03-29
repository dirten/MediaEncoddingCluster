#ifndef ORG_ESB_IO_BUFFEREDOUTPUTSTREAM_H
#define ORG_ESB_IO_BUFFEREDOUTPUTSTREAM_H
#include "OutputStream.h"
#include <vector>
#include "org/esb/io/exports.h"
namespace org{
namespace esb{
namespace io{
class BufferedOutputStream: public OutputStream{
	public:
		BufferedOutputStream(OutputStream * os);
		BufferedOutputStream(OutputStream* os, int bufferSize);
		~BufferedOutputStream();
		void write(char  buffer);
		void write(char * buffer, int length);
		void write(vector <unsigned char >& buffer);
		bool close();
		void flush();
	private:
		int _buffer_size;
		char * _buffer;
		int _count;
		OutputStream * _os;
};
}}}
#endif
