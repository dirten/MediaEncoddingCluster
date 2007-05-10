#ifndef ORG_ESB_UTIL_STRINGUTIL_H
#define ORG_ESB_UTIL_STRINGUTIL_H
#include <string>
#include <iostream>
using namespace std;
namespace org{
    namespace esb{
	namespace util{

	    class StringUtil{
		public:
		    string tmp;
		    static const char * trim(string & s, string & drop = *new string(" "));
	    };

	}
    }
}
#endif
