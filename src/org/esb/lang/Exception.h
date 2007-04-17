

#ifndef ORG_ESB_LANG_EXCEPTION_H
#define ORG_ESB_LANG_EXCEPTION_H
#include <iostream>
namespace org{
    namespace esb{
	namespace lang{
	    class Exception{
		public:
    		    Exception();
		    Exception(const char * filename, const int linenumber, const char * message, ...);
	    };
	}
    }
}
#endif


