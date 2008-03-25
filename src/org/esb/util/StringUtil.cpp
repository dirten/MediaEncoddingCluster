#include "StringUtil.h"


namespace org{
    namespace esb{
	namespace util{	
	    const char* StringUtil::trim(string & s, string &drop ){
		string r=s.erase(s.find_last_not_of(drop)+1);
		return r.erase(0,r.find_first_not_of(drop)).c_str();
	    }
	}
    }
}

