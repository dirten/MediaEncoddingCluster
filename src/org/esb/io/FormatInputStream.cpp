#include "FormatInputStream.h"
#include <iostream>
using namespace std;

//using namespace org::esb::io;
namespace org{
    namespace esb{
	namespace io{
	    bool isInitialized=false;
	    FormatInputStream::FormatInputStream(File * source){
		initialize();

	    }

	    FormatInputStream::~FormatInputStream(){


	    }

	    int FormatInputStream::available(bool isBlocking){
		return 0;
	    }

	    int FormatInputStream::read(unsigned char * buffer, int length){
		return 0;
	    }
	    
	    void FormatInputStream::close(){
	    
	    }

	    void FormatInputStream::initialize(){
		if(!isInitialized){
		    cout << "static initializer from FormatInputStream"<<endl;
		    isInitialized=true;
		}
	    }

	}
    }
}


