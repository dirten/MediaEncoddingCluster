#include "StringUtil.h"


namespace org {
  namespace esb {
    namespace util {

      const std::string StringUtil::trim(string & s, string &drop) {
        string r = s.erase(s.find_last_not_of(drop) + 1);
        return r.erase(0, r.find_first_not_of(drop));
      }

      const std::string StringUtil::toLower(std::string s) {
        std::string result;
        const char * str = s.c_str();
        for (unsigned int i = 0; i < strlen(str); i++) {
          if (str[i] >= 'A' && str[i] <= 'Z')
            result += str[i] | 32;
          else
            result += str[i];
        }
        return result;
      }
    }
  }
}

