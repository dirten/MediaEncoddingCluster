#ifndef ORG_ESB_IO_OBJECTINPUTSTREAM
#define ORG_ESB_IO_OBJECTINPUTSTREAM
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <iostream>
//#include <ostream>
//#include <sstream>
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
		    int64_t length=0;;
		    _is->read((unsigned char*)&length,sizeof(int64_t));
		    if(!length>0)cout <<"Fehler in der groesse INBOUND_DATA:"<<length<<endl;
		    char in[length+1];
		    
		    memset(&in,0,length+1);
		    int readed=_is->read((unsigned char*)&in,length);
//		    ofstream of("tmp.data",ios::binary);
//		    of.write((char*)&inbound_data,length);
//		    archive_stream.str((char*)inbound_data);
//		    ifstream is("tmp.data",ios::binary);
		    istringstream archive_stream((char*)&in);
//		    archive_stream.str(inbound_data);
		    boost::archive::text_iarchive archive(archive_stream);
		    archive >> object;
//		    delete [] inbound_data;
		}
		private:
		    InputStream * _is;
	    };
	}
    }
}
#endif
