#include <stdio.h>
#include "Version.h"
#include "org/esb/lang/Exception.h"

namespace org {
  namespace esb {
    namespace core {
      using org::esb::lang::Exception;

      Version::Version(){
        init(MHIVE_VERSION);
      }

      Version::Version(std::string version)
      {
        init(version);
      }

      void Version::init(std::string version_string)
      {
        if(sscanf(version_string.c_str(),"%d.%d.%d", &major, &minor, &patch)!=3){
          throw Exception("no version string found:"+version_string);
        }
      }

      std::string Version::toString(){
        std::stringstream sstream;
        sstream << major << "." << minor << "." <<patch;
        return sstream.str();
      }

      bool Version::equals(const Version & a)const{
        return compare(a)==0;
      }

      bool Version::newerAs(Version & version){
        return this->compare(version)>0;
      }

      bool Version::olderAs(Version & version){
        return this->compare(version)<0;
      }

      int Version::compare(const Version & version)const {
        int result=0;
        if(major!=version.major){
          result=major>version.major?1:-1;
        }else if(minor!=version.minor){
          result=minor>version.minor?1:-1;
        }else if(patch!=version.patch){
          result=patch>version.patch?1:-1;
        }
        return result;
      }

      std::ostream& operator << (std::ostream& stream, Version const& obj)
      {
        stream<<obj.major<<"."<<obj.minor<<"."<<obj.patch;
        return stream;
      }

      }
    }
  }
