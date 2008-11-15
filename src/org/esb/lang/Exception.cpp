#include <stdio.h>
#include "org/esb/lang/Exception.h"
#ifndef WIN32
#include <execinfo.h>
#endif
using namespace org::esb::lang;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
/**
*	
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
    //logger.log( message );   
}

////////////////////////////////////////////////////////////////////////////////
void Exception::setMark( const char* file, const int lineNumber ){
    
    // Add this mark to the end of the stack trace.
    stackTrace.push_back( std::make_pair( (std::string)file, (int)lineNumber ) );
    
    ostringstream stream;
    stream << "\tFILE: " << stackTrace[stackTrace.size()-1].first;
    stream << ", LINE: " << stackTrace[stackTrace.size()-1].second;

    //logger.log( stream.str() );    
}
#ifdef BACKTRACE
#ifndef WIN32
const char * Exception::what()const throw(){
    string msg= getStackTraceString();
       size_t size;
       char **strings;
       size_t i;
	const void * return_addr=__builtin_return_address(1);
	cout <<"Return addr"<<return_addr<<endl;
       size = backtrace ((void **)this, sizeof(Exception));
//       size = backtrace ((void **)this, 100);
       strings = backtrace_symbols ((void *const *)this, size);
     
       printf ("Obtained %zd stack frames.\n", size);
     
       for (i = 0; i < size; i++)
          printf ("%s\n", strings[i]);
     
       free (strings);

    return msg.c_str();
}
#else
const char * Exception::what()const throw(){
  return getStackTraceString().c_str();
}
#endif
#endif

