#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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
		    throw Exception(__FILE__, __LINE__, "File::File - Filename given is NULL");
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

	    bool File::exist(){
		struct stat attribute;
		return stat(getPath(), &attribute)==0;
	    }
	    
	    bool File::isFile(){
		struct stat attribute=getStat(getPath());
		return S_ISREG(attribute.st_mode);
	    }
	    
	    bool File::isDirectory(){
		struct stat attribute=getStat(getPath());
		return S_ISDIR(attribute.st_mode);
	    }
	    
	    bool File::canRead(){
		struct stat attribute=getStat(getPath());
		uid_t uid=getuid();
		gid_t gid=getgid();
		return ((attribute.st_mode&S_IRUSR&&attribute.st_uid==uid)
			||(attribute.st_mode&S_IRGRP&&attribute.st_gid==gid)
			||attribute.st_mode&S_IROTH);
	    }
	    
	    bool File::canWrite(){
		struct stat attribute=getStat(getPath());
		uid_t uid=getuid();
		gid_t gid=getgid();
		return ((attribute.st_mode&S_IWUSR&&attribute.st_uid==uid)
			||(attribute.st_mode&S_IWGRP&&attribute.st_gid==gid)
			||attribute.st_mode&S_IWOTH);
	    }
	}
    }
}

