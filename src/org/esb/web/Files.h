#include <stdlib.h>

#include "org/esb/httpd/shttpd.h"
#include "org/esb/util/Properties.h"

using namespace org::esb::util;
namespace org{
namespace esb{
namespace web{
    class Files{
	public:
	    static void upload_file(struct shttpd_arg *arg);
	    static void show_files(struct shttpd_arg *arg);
	    static void show_files(struct shttpd_arg *arg, Properties& params);
	    static void show_details(struct shttpd_arg *arg, Properties& params);
	    static void edit_details(struct shttpd_arg *arg, int fileid);
    };
}}}

