#ifndef _BMPUTIL_H
#define	_BBMPUTIL_H
#include <string>
namespace org {
  namespace esb {
    namespace av {
      class Frame;
      class BMPUtil{
      public:
        static Frame * load(std::string file);
        static void save(std::string file, Frame *);
      };

    }
  }
}
#endif
