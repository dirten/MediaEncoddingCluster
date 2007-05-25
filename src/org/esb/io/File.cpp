#include <iostream>
#include "File.h"
using namespace std;




namespace org{
    namespace esb{
	namespace io{	    
	    File::File(char * filename){
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
	}
    }
}

