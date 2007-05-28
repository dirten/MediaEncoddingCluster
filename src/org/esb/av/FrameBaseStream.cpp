#include "FormatBaseStream.h"
#include <iostream>


namespace org{
    namespace esb{
	namespace io{
	    bool isInitialized=false;
	    void FormatBaseStream::initialize(){
		if(!isInitialized){
		    std::cout << "static initializer from FormatBaseStream"<<std::endl;
		    isInitialized=true;
		}
	    }	
	}
    }
}


