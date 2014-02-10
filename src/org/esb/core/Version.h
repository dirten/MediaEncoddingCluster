#ifndef VERSION_H
#define VERSION_H
#include <string>
namespace org {
  namespace esb {
    namespace core {

      class Version
      {
      public:
        Version();
        Version(std::string version);
        bool newerAs(Version & version);
        bool olderAs(Version & version);
        bool equals(const Version & version)const;

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
