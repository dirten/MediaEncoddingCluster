#include "FrameBaseStream.h"
#include <iostream>


namespace org{
    namespace esb{
	namespace io{
	    bool isInitialized=false;
	    void FrameBaseStream::initialize(){
		if(!isInitialized){
		    std::cout << "static initializer from FrameBaseStream"<<std::endl;
		    isInitialized=true;
		}
	    }	
	}
    }
}


