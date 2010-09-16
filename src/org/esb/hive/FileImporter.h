#ifndef ORG_ESB_HIVE_FILEIMPORTER_H
#define ORG_ESB_HIVE_FILEIMPORTER_H

#include "org/esb/io/File.h"

namespace db{
  class HiveDb;
}
namespace org {
  namespace esb {
    namespace hive {
      class FileImporter {
      public:
        FileImporter();
        db::MediaFile import(org::esb::io::File);
        Ptr<db::HiveDb> getConnection();
      private:
        Ptr<db::HiveDb> _connection;
      };
    }
  }
}
#endif
