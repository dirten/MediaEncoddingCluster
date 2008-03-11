#include "Page.h"
#include "Files.h"
#include "Stream.h"
#include "org/esb/util/StringTokenizer.h"


using namespace org::esb::web;
using namespace org::esb::util;

void Page::showPage(struct shttpd_arg *arg){
    shttpd_printf(arg, "<html><head>"
    	    "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"><head>"
	    "<body>");
    shttpd_printf(arg, "<div class=\"menu\">"
	    "<div class=\"menuentry\"><a href=\"?page=files\">Files</a></div>"
	    "<div class=\"menuentry\"><a href=\"?page=jobs\">Jobs</a></div>"
	    "</div>");
    Properties param=getParams(arg);
    if(!param.hasProperty("page")){
	Files::show_files(arg, param);
    }else if(strcmp(param.getProperty("page"),"files")==0){
	Files::show_files(arg, param);    
    }else if(param.hasProperty("edit_stream")){
	Stream::edit_stream(arg, param);
    }
    shttpd_printf(arg,"</body></html>");
    arg->flags |= SHTTPD_END_OF_OUTPUT;
}

Properties Page::getParams(struct shttpd_arg *arg){
    Properties props;
    const char * query_string = shttpd_get_env(arg, "QUERY_STRING");
    if(query_string!=NULL){
	StringTokenizer st(query_string,"&");
	while(st.hasMoreTokens()){
	    string s=st.nextToken();
	    StringTokenizer st2(s,"=");
	    string key=st2.nextToken();
	    string val;
	    if(st2.countTokens()==1){
	        val=st2.nextToken();
	    }
	props.setProperty(key.c_str(),val.c_str());
	}
    }
  return props;
}

