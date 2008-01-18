#include "ObjectOutputStream.h"


using namespace org::esb::io;

ObjectOutputStream::ObjectOutputStream(OutputStream * os){
    _os=os;
}

	void ObjectOutputStream::write(char  buffer){}
	void ObjectOutputStream::write(char * buffer, int length){}
	void ObjectOutputStream::write(vector <unsigned char >& buffer){}
	void ObjectOutputStream::close(){}
	void ObjectOutputStream::flush(){}


