/* 
 * File:   XmlApp.h
 * Author: HoelscJ
 *
 * Created on 6. Januar 2011, 15:05
 */

#ifndef XMLAPP_H
#define	XMLAPP_H
#include "Wt/WResource"
#include "Wt/WEnvironment"
#include "Wt/Http/Request"
#include "Wt/Http/Response"

namespace org {
  namespace esb {
    namespace web {

      class XmlApp : public Wt::WResource {
      public:
        XmlApp();
        virtual ~XmlApp();
        void handleRequest(const Wt::Http::Request&, Wt::Http::Response&);
        void getData(uint64_t,uint64_t);
      private:

      };
    }
  }
}
#endif	/* XMLAPP_H */

