#include <iostream>
#include <assert.h>
#include "File.h"
#include "org/esb/lang/Exception.h"
using namespace std;
using namespace org::esb::lang;



namespace org{
    namespace esb{
	namespace io{	    
	    File::File(const char * filename){
	    assert(filename!=NULL);
//		if(this->_filename)
//		    delete _filename;
//		_filename=new char[strlen(filename)];
//		memcpy((char*)_filename, filename, strlen(filename));
		_filename=filename;
	    }

	    File::File(){

	    }

	    File::~File(){
//		if(_filename)
//		    free((void*)_filename);
	    }
	    
	    const char * File::getPath(){
		return _filename;
	    }
	    
	    bool File::isFile(){
		throw Exception(__FILE__, __LINE__, "Not implemented!!! ");
		return false;
	    }
	    
	    bool File::isDirectory(){
		return false;
	    }
	}
    }
}

