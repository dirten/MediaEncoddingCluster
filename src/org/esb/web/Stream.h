#include "org/esb/httpd/shttpd.h"



namespace org{
namespace esb{
namespace web{
    class Stream{
	public:
	    static void show_stream(struct shttpd_arg *arg, Properties & props);
	    static void edit_stream(struct shttpd_arg *arg, Properties & props);
	    static void build_stream_form(struct shttpd_arg *arg, Properties & props);
//	    static void build_stream_form(struct shttpd_arg *arg, Properties & props);    
    };
}}}


