#ifndef ORG_ESB_IO_OBJECTINPUTSTREAM
#define ORG_ESB_IO_OBJECTINPUTSTREAM
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <sstream>
#include "ObjectStream.h"
#include "InputStream.h"
using namespace std;
namespace org{
    namespace esb{
	namespace io{
	    class ObjectInputStream:public InputStream{
		public:
		ObjectInputStream(InputStream * is);
		int available(bool isBlocking=false);
		int read(unsigned char * buffer, int length);
		int read(vector<unsigned char>&buffer);
		int read();

	    	template<typename T>
		void readObject(T& object){
		    istringstream archive_stream;
		    int length=0;;

		    _is->read((unsigned char*)&length,sizeof(int));
		    cout << "inbound data length:"<<length<<endl;
		    unsigned char inbound_data[length];
//		    memcpy(inbound_data,0,length);
		    
		    int readed=_is->read((unsigned char*)&inbound_data,length);
//		    readed=_is->read((unsigned char*)&inbound_data,length);
		    string inbound_string=(const char*)&inbound_data;
		    cout << "inbound data readed:"<<readed<<endl;
		    archive_stream.str((char*)inbound_data);
		    boost::archive::text_iarchive archive(archive_stream);
		    archive >> object;
		}
		private:
		    InputStream * _is;
	    };
	}
    }
}
#endif
