#include "ObjectOutputStream.h"
#include "org/esb/io/BufferedOutputStream.h"

using namespace org::esb::io;

ObjectOutputStream::ObjectOutputStream(OutputStream * os){
//    _os=new BufferdOutputStream(os, 32000);
    _os=os;
}

	void ObjectOutputStream::write(char  buffer){}
	void ObjectOutputStream::write(char * buffer, int length){}
	void ObjectOutputStream::write(vector <unsigned char >& buffer){}
	void ObjectOutputStream::close(){}
	void ObjectOutputStream::flush(){}


