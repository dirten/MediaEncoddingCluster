#ifndef VERSION_H
#define VERSION_H
#include <string>
#include "exports.h"
namespace org {
  namespace esb {
    namespace core {

      class CORE_EXPORT Version
      {
      public:
        Version();
        Version(std::string version);
        bool newerAs(Version & version);
        bool olderAs(Version & version);
        bool equals(const Version & version)const;
        std::string toString();
        int major;
        int minor;
        int patch;
        friend std::ostream& operator << (std::ostream& stream, Version const& obj);
      private:
        void init(std::string version_string);
        int compare(const Version & version)const;
      };
      }
    }
  }
#endif // VERSION_H
