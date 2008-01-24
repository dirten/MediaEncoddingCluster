#ifndef ORG_ESB_IO_OBJECTOUTPUTSTREAM_H
#define ORG_ESB_IO_OBJECTOUTPUTSTREAM_H
#include "OutputStream.h"
#include "ObjectStream.h"
#include <boost/lexical_cast.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/array.hpp>
#include <fstream>

namespace org{
    namespace esb{
	namespace io{
class ObjectOutputStream:public OutputStream{
    public:
	ObjectOutputStream(OutputStream * out);
	
	void write(char  buffer);
	void write(char * buffer, int length);
	void write(vector <unsigned char >& buffer);
	void close();
	void flush();

	template<typename T>
	void writeObject(const T&object){
//	    char test[10];
//	    boost::iostreams::basic_array_sink sink((char*)&test,10);
	    std::ostringstream archive_stream;
	    boost::archive::text_oarchive archive(archive_stream);
	    archive << object;
	    std::string _outbound_data = archive_stream.str();
	    int length=_outbound_data.length();
	    _os->write((char*)&length,sizeof(int64_t));
	    _os->write((char*)_outbound_data.c_str(),_outbound_data.length());
	}
	
    private:
	OutputStream * _os;
};
}}}
#endif
