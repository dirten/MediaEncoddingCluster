#include "org/esb/util/Properties.h"
#include "org/esb/io/File.h"
namespace org {
  namespace esb {
    namespace config {
      void loadDefaults(Properties * props) {
        /*default port for HiveListener*/
        props->setProperty("hive.port", "20200");
        /*default port for Webserver*/
        props->setProperty("web.port", "8080");

      }
    }
  }
}
