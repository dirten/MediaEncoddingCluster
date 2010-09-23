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
        /*default url for the Database Server*/
        props->setProperty("db.url", "host=localhost;database=hive;user=root;port=3306");

        /*default parameter for the encoding profiles*/
        props->setProperty("framerates", "1/10#10,1/15#15,1000/23976#23.976,1/24#24,1/25#25,100/2997#29.97,1/30#30");
        props->setProperty("samplerates", "8000,11025,22050,32000,44100,48000");
        props->setProperty("audiobitrates", "32000,96000,128000,160000,192000,224000,256000,320000");

        props->setProperty("MYSQLD_BIN","./mysqld");
        props->setProperty("MYSQL_DATA","../data");
        props->setProperty("MYSQL_LANG","../res");
        props->setProperty("MYSQL_BOOT","../sql/CreateDatabase.sql");
        /*setting up the base path*/
        if(props->getProperty("hive.base_path")==NULL){
        org::esb::io::File base("..");
        LOGDEBUG("BasePath=" << base.getPath());
        string base_path = base.getPath();
        props->setProperty("hive.base_path", base_path.c_str());
        }

      }
    }
  }
}
