/* 
 * File:   SetupPage.h
 * Author: HoelscJ
 *
 * Created on 20. Dezember 2010, 13:08
 */

#ifndef SETUPPAGE_H
#define	SETUPPAGE_H

#include "Wt/WApplication"
namespace org {
  namespace esb {
    namespace web {

      class SetupPage: public Wt::WApplication {
      public:
        SetupPage(const Wt::WEnvironment & env);
        virtual ~SetupPage();
      private:

      };
    }
  }
}

#endif	/* SETUPPAGE_H */

