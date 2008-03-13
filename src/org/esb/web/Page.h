#include <stdlib.h>
#include "org/esb/httpd/shttpd.h"
#include "org/esb/util/Properties.h"

using namespace org::esb::util;
namespace org{
namespace esb{
namespace web{
    class Page{
	public:
//	    Page();
//	    void setPage(shttpd_callback_t  arg);
//	    void show_page(shttpd_callback_t  arg);
	    static void showPage(struct shttpd_arg *arg);
	private:
	    static Properties getParams(struct shttpd_arg *arg);
	    Properties _params;
    };
}}}


