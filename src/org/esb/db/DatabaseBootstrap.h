/* 
 * File:   DatabaseBootstrap.h
 * Author: HoelscJ
 *
 * Created on 20. September 2010, 16:58
 */

#ifndef DATABASEBOOTSTRAP_H
#define	DATABASEBOOTSTRAP_H
namespace org {
  namespace esb {
    namespace db {

      class DatabaseBootstrap {
      public:
        static void bootstrap();
      private:
        DatabaseBootstrap();
        DatabaseBootstrap(const DatabaseBootstrap& orig);
        virtual ~DatabaseBootstrap();

      };
    }
  }
}

#endif	/* DATABASEBOOTSTRAP_H */

