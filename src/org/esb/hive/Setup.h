/* 
 * File:   Setup.h
 * Author: jhoelscher
 *
 * Created on 5. November 2008, 13:25
 */
#include <string>
#include "org/esb/util/Log.h"
#ifndef _ORG_ESB_HIVE_SETUP_H
#define	_ORG_ESB_HIVE_SETUP_H
namespace org{
  namespace esb{
    namespace hive{
      class Setup{
        classlogger("org.esb.hive.Setup")
      public:
        static void check();
//      private:
        static bool checkDatabaseExist();
        static bool checkDatabaseVersion(std::string version);
        static void buildDatabase(std::string dbName);
        static void buildDatabaseModel(std::string sqlfile);
        static void upgradeDatabaseModel(std::string sqlfile);
        static std::string parseConnectionString(std::string, std::string);
        static bool yesNoQuestion(std::string quest);
      };
    }
  }
}


#endif	/* _SETUP_H */

