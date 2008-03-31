#include "org/esb/httpd/shttpd.h"



namespace org{
namespace esb{
namespace web{
    class Stream{
	public:
	    static void show_input_stream(struct shttpd_arg *arg, Properties & props);
	    static void show_output_stream(struct shttpd_arg *arg, Properties & props);
	    static void save_output_stream(struct shttpd_arg *arg);
//	    static void build_stream_form(struct shttpd_arg *arg, Properties & props);
//	    static void build_stream_form(struct shttpd_arg *arg, Properties & props);    
    };
}}}


