#include "ObjectOutputStream.h"
#include "org/esb/io/BufferedOutputStream.h"

using namespace org::esb::io;

ObjectOutputStream::ObjectOutputStream(OutputStream * out){
//    _os=new BufferedOutputStream(out, 32000);
    _os=out;
}

	void ObjectOutputStream::write(char  buffer){}
	void ObjectOutputStream::write(char * buffer, int length){}
	void ObjectOutputStream::write(vector <unsigned char >& buffer){}
	bool ObjectOutputStream::close(){
	    _os->flush();
	    _os->close();
            return true;
	}
	void ObjectOutputStream::flush(){}


