#include "org/esb/util/Properties.h"
#include "org/esb/io/File.h"
namespace org {
  namespace esb {
    namespace config {
      void loadDefaults(Properties * props) {
        /*setting up the base path*/
        std::string base_path;
        if(!props->hasProperty("hive.base_path")){
          org::esb::io::File base(".");
          base_path = base.getPath();
        }else{
          base_path = props->getProperty("hive.base_path");
        }
        std::cout<<"BasePath=" << base_path<<std::endl;
        props->setProperty("hive.base_path", base_path.c_str());
        props->setProperty("hive.data_path", base_path+"/data");

        /*default port for HiveListener*/
        props->setProperty("hive.port", "20200");

        /*default port for Webserver*/
        props->setProperty("web.port", "8080");

        /*make directory for the database*/
        org::esb::io::File datadir(base_path+"/data");
        if(!datadir.exists())
          datadir.mkdir();
        /*default url for the Database Server*/
        props->setProperty("db.url", "database="+base_path+"/data/hive.db");

        /*default parameter for the encoding profiles*/
        props->setProperty("framerates", "1/10#10,1/15#15,1000/23976#23.976,1/24#24,1/25#25,100/2997#29.97,1/30#30");
        props->setProperty("samplerates", "8000,11025,22050,32000,44100,48000");
        props->setProperty("audiobitrates", "32000,96000,128000,160000,192000,224000,256000,320000");

        

      }
    }
  }
}
