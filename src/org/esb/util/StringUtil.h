#ifndef ORG_ESB_UTIL_STRINGUTIL_H
#define ORG_ESB_UTIL_STRINGUTIL_H
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
namespace org {
    namespace esb {
        namespace util {

            class StringUtil {
            public:
                string tmp;
                static const std::string trim(string & s, string & drop = *new string(" "));
                static const std::string toLower(std::string);
//                static const std::string toString(int);


                template <class T> std::string toString(const T &t) {
                    std::ostringstream oss;
                    oss << t;
                    return std::string(oss.str());
                }
            };

        }
    }
}
#endif
