#include "FileInputStream.h"
#include "org/esb/lang/Exception.h"
#include <iostream>
using namespace std;
using namespace org::esb::lang;
namespace org{
    namespace esb{
	namespace io{	    
	    void FileInputStream::open(const char * name){
		FILE * f=fopen(name,"r+b");
		if(!f){
		    throw Exception(__FILE__, __LINE__, "FileInputStream::open - File not Found");
		}
	    }
	    void FileInputStream::close(){
		
	    }
	    int FileInputStream::available(bool isBlocking){
		return 0;
	    }

	    int FileInputStream::read(unsigned char * buffer, int length){
		return 0;	
	    }
	    
	    FileInputStream::FileInputStream(File * file)throw (Exception){
		const char * name=file!=NULL?file->getPath():NULL;
		if(!name){
		    throw Exception(__FILE__, __LINE__, "FileInputStream::FileInputStream - No Filename given ");
		}
		open(name);
	    }
	}
    }
}


