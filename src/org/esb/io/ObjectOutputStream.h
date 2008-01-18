#ifndef ORG_ESB_IO_OBJECTOUTPUTSTREAM_H
#define ORG_ESB_IO_OBJECTOUTPUTSTREAM_H
#include "OutputStream.h"
#include <boost/archive/text_oarchive.hpp>
#include <istream>

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
	void write_object(const T&object){
	    std::ostringstream archive_stream;
	    boost::archive::text_oarchive archive(archive_stream);
	    archive << object;
	    _outbound_data = archive_stream.str();
	    _os->write((char*)_outbound_data.c_str(),_outbound_data.length());
	}
    private:
	OutputStream * _os;
	std::string _outbound_data;
};
}}}
#endif
