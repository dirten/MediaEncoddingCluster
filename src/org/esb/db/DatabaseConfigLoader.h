/* 
 * File:   DatabaseConfigLoader.h
 * Author: HoelscJ
 *
 * Created on 20. September 2010, 14:52
 */

#ifndef DATABASECONFIGLOADER_H
#define	DATABASECONFIGLOADER_H
namespace org {
  namespace esb {
    namespace db {

      class DatabaseConfigLoader {
      public:
        static void load();
        static void save();
      private:
        DatabaseConfigLoader();
        DatabaseConfigLoader(const DatabaseConfigLoader& orig);
        ~DatabaseConfigLoader();
      };
    }
  }
}

#endif	/* DATABASECONFIGLOADER_H */

