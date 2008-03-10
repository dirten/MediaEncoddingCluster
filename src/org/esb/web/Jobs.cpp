#include "Jobs.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include <iostream>

using namespace org::esb::sql;
using namespace org::esb::config;
using namespace org::esb::web;
void Jobs::show_jobs(struct shttpd_arg *arg){

	char value[20];
	const char * query_string = shttpd_get_env(arg, "QUERY_STRING");
	if(query_string!=NULL)
	    shttpd_get_var("name", query_string, strlen(query_string), value, sizeof(value));
	

	Connection con(Config::getProperty("db.connection"));
	Statement stmt=con.createStatement("select * from files");
	ResultSet rs=stmt.executeQuery();

	shttpd_printf(arg, "<div>");
	while(rs.next()){
	    shttpd_printf(arg, "<div>");
	    shttpd_printf(arg, rs.getstring(1).c_str());
	    shttpd_printf(arg, "</div>");
	}
	shttpd_printf(arg, "</div>");
	arg->flags |= SHTTPD_END_OF_OUTPUT;


}
