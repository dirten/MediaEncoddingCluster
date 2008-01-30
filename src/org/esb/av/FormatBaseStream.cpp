#include "FormatBaseStream.h"
extern "C" {
#include "avformat.h"
}
#include <iostream>


namespace org{
    namespace esb{
	    namespace av{
	        
	        bool isInitialized=false;
	        
	        FormatBaseStream::FormatBaseStream(){
            	    initialize();
	        }
	        
	        void FormatBaseStream::initialize(){
		        if(!isInitialized){
		            av_register_all();
		            isInitialized=true;
		        }
	        }	
	    }
    }
}


