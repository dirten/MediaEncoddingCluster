#include <stdio.h>
#include "org/esb/lang/Exception.h"


using namespace org::esb::lang;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
/**
*	TestBescreibung
*
*/

void Exception::buildMessage(const char* format, va_list& vargs)
{
    // Allocate buffer with a guess of it's size
    int size = 128;
    
    // Format string
    while( true ){
    	
    	// Allocate a buffer of the specified size.
    	char* buffer = new char[size];
    	
        int written = vsnprintf(buffer, size, format, vargs);
        if (written > -1 && written < size-1) {
            
            // Guessed size was enough. Assign the string.
            message.assign (buffer, written);
            
            // assign isn't passing ownership, just copying, delete
            // the allocated buffer.
            delete [] buffer;
            
            break;
        }
                
        // Our buffer wasn't big enough - destroy the old buffer,
        // double the size and try again.
        delete [] buffer;
        size *= 2;
    }
    
    //activemq::logger::SimpleLogger logger("com.yadda1");
    //logger.log( message );   
}

////////////////////////////////////////////////////////////////////////////////
void Exception::setMark( const char* file, const int lineNumber ){
    
    // Add this mark to the end of the stack trace.
    stackTrace.push_back( std::make_pair( (std::string)file, (int)lineNumber ) );
    
    ostringstream stream;
    stream << "\tFILE: " << stackTrace[stackTrace.size()-1].first;
    stream << ", LINE: " << stackTrace[stackTrace.size()-1].second;
                 
    //activemq::logger::SimpleLogger logger("com.yadda2");
    //logger.log( stream.str() );    
}



