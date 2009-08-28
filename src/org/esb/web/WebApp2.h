/* 
 * File:   WebApp2.h
 * Author: jhoelscher
 *
 * Created on 26. August 2009, 13:13
 */

#include <Wt/WApplication>
#include <Wt/WEnvironment>

#ifndef _WEBAPP2_H
#define	_WEBAPP2_H
namespace org {
    namespace esb {
        namespace web {

            class WebApp2 : public Wt::WApplication {
            public:
                WebApp2(const Wt::WEnvironment & env);
            private:
            };
        }
    }
}
#endif	/* _WEBAPP2_H */

