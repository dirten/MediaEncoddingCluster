#include "FormatBaseStream.h"

#include "AV.h"

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


