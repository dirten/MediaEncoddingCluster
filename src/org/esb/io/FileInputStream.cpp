#include "FileInputStream.h"
#include "org/esb/lang/Exception.h"
#include <iostream>
//#include <sys/ioctl.h>


using namespace std;
using namespace org::esb::lang;
namespace org{
    namespace esb{
	namespace io{	    
	    void FileInputStream::open(const char * name){
		file=fopen(name,"r+b");
		
		if(!file){
		    string error="FileInputStream::open - File not Found (";
		    error+=name;
		    error+=")";
		    throw Exception(__FILE__, __LINE__, error.c_str());
		}
	    }
	    void FileInputStream::close(){
		fclose(file);
	    }
	    
	    int FileInputStream::available(bool isBlocking){
		long int currentPosition=ftell(file);
		fseek (file, 0, SEEK_END);
		long int filesize= ftell(file);
		fseek (file, 0, currentPosition);
	    	return filesize;
	    }

	    int FileInputStream::read(unsigned char * buffer, int length){
		fgets((char*)buffer, length, file);
		return length;
	    }
	    
	    FileInputStream::FileInputStream(File * file)throw (Exception){
		const char * name=file!=NULL?file->getPath():NULL;
		if(!name){
		    throw Exception(__FILE__, __LINE__, "FileInputStream::FileInputStream - No Filename given ");
		}
		open(name);
	    }
	    FileInputStream::~FileInputStream(){
		this->close();
	    }
	}
    }
}


