#ifndef ORG_ESB_UTIL_STRINGUTIL_H
#define ORG_ESB_UTIL_STRINGUTIL_H
#include <string>
#include <iostream>
#include <sstream>
#include "exports.h"
using namespace std;
namespace org {
  namespace esb {
    namespace util {

      class UTIL_EXPORT StringUtil {
      public:
        string tmp;
        static const std::string trim(string s, string drop = string(" "));
        static const std::string toLower(std::string);

        template <class T>
        static std::string toString(const T &t) {
          std::ostringstream oss;
          oss << t;
          return std::string(oss.str());
        }

        static std::string toString(const std::wstring &t) {
          return std::string(t.begin(), t.end());
        }

        template <class T>
        static std::wstring toWString(const T &t) {
          std::string str = toString(t);
          return std::wstring(str.begin(), str.end());
        }

        static std::wstring toWString(const std::string &t) {
          return std::wstring(t.begin(), t.end());
        }

        template <class T, class A>
        static T join(const A &begin, const A &end, const T &t){
          T result;
          for (A it=begin; it!=end; ++it){
            if (!result.empty()){
              result.append(t);
            }
            result.append(*it);
          }
          return result;
        }

        static std::string replace(std::string s, const std::string& needle,const std::string& replace);
      };

      }
    }
  }
#endif
