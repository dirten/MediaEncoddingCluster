#include "StringUtil.h"

#include <string.h>
#include <locale>

namespace org {
  namespace esb {
    namespace util {

      const std::string StringUtil::trim(string s, string  drop) {
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
      std::string StringUtil::replace(std::string s, const std::string& needle,const std::string& replace) {
          unsigned int position = s.find(needle); 
          while (position != std::string::npos) {
            s.erase(position, needle.length());
            s.insert(position, replace);
            position = s.find(needle, position + 1);
          }
          return s;
        }

/*
      template <class T> std::string StringUtil::toString(const T t) {
        std::ostringstream oss;
        oss << t;
        return std::string(oss.str());
      }

      template <> std::string StringUtil::toString(const std::wstring t) {
        return std::string(t.begin(), t.end());
      }
*/
      /*
      const std::string StringUtil::toString(int num) {
        char c[10];
        memset(&c, 0, 10);
        sprintf(c, "%d", num);
        return string(c);
      }
       */
    }
  }
}

