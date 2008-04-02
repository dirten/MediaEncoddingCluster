#ifndef ORG_ESB_WEB_PROFILES_H
#define ORG_ESB_WEB_PROFILES_H
#include <stdlib.h>
#include "org/esb/httpd/shttpd.h"

namespace org{
namespace esb{
namespace web{
    class Profiles{
	public:
    	    static void list(struct shttpd_arg *arg);
    	    static void edit(struct shttpd_arg *arg);
    	    static void save(struct shttpd_arg *arg);
    };
}}}
#endif

