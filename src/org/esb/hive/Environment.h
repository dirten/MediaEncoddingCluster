/* 
 * File:   Environment.h
 * Author: HoelscJ
 *
 * Created on 2. November 2011, 13:22
 */

#ifndef ENVIRONMENT_H
#define	ENVIRONMENT_H
namespace org {
  namespace esb {
    namespace hive {

      class Environment {
      public:
        static void build(int argc, char**argv);
      private:
        Environment();
        virtual ~Environment();

      };
    }
  }
}

#endif	/* ENVIRONMENT_H */

