#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include "File.h"
#include "org/esb/lang/Exception.h"
using namespace std;
using namespace org::esb::lang;



namespace org{
    namespace esb{
	namespace io{
	    struct stat getStat(const char * filename){
		struct stat attribute;
		stat(filename, &attribute);
		return attribute;
	    
	    }
	    
	    File::File(const char * filename){
		if(filename==NULL){
		    throw Exception(__FILE__, __LINE__, "File::File - Filename given ");

		}
		_filename=filename;
	    }

	    File::File(){

	    }

	    File::~File(){
	    }
	    
	    const char * File::getPath(){
		return _filename;
	    }
	    
	    bool File::isFile(){
		struct stat attribute=getStat(getPath());
		return S_ISREG(attribute.st_mode);
	    }

	    bool File::exist(){
		struct stat attribute=getStat(getPath());
		return attribute.st_ino>0;
	    }
	    
	    bool File::isDirectory(){
		struct stat attribute=getStat(getPath());
		return S_ISDIR(attribute.st_mode);
	    }
	}
    }
}

