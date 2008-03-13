#include "Files.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include "Stream.h"
#include <iostream>

using namespace org::esb::web;
using namespace org::esb::sql;
using namespace org::esb::config;
//using namespace org::esb::utils;


void Files::show_files(struct shttpd_arg *arg, Properties & params){

    if(params.hasProperty("filedetails")){
	show_details(arg,params);
    }else{
    Connection con(Config::getProperty("db.connection"));
    Statement stmt=con.createStatement("select * from files");
    ResultSet rs=stmt.executeQuery();
    shttpd_printf(arg, "<div>");
    while(rs.next()){
        shttpd_printf(arg, "<div><a href=\"?filedetails=%d\">%s</a></div>",rs.getint(0),rs.getstring(1).c_str());
    }
    shttpd_printf(arg, "</div>");
    }
}

void Files::show_details(struct shttpd_arg *arg, Properties & props){
    int id=atoi(props.getProperty("filedetails"));
    Connection con(Config::getProperty("db.connection"));
    {
	Statement stmt=con.createStatement("select id from streams where fileid=? order by stream_index");
//	stmt.bind(1,"1");
	stmt.bind(1,id);
        ResultSet rs=stmt.executeQuery();
//        int cc=rs.getColumnCount();
        while(rs.next()){
    	    props.setProperty("streamid",rs.getstring(0));
    	    Stream::show_stream(arg, props);
	}
    }
    {
	Statement stmt=con.createStatement("select s.id from jobs j, job_details jd, streams s where infile=? and j.id=jd.job_id and jd.outstream=s.id;");
	stmt.bind(1,id);
        ResultSet rs=stmt.executeQuery();
//        int cc=rs.getColumnCount();
        while(rs.next()){
    	    props.setProperty("streamid",rs.getstring(0));
    	    Stream::edit_stream(arg, props);
	}
    }
    shttpd_printf(arg, "<div>");
    shttpd_printf(arg, "<a href=\"?fileid=%d&page=streams&edit_stream=-1\">Add Stream</a>", id);
    shttpd_printf(arg, "</div>");

}

void Files::edit_details(struct shttpd_arg *arg, int fileid){
    Connection con(Config::getProperty("db.connection"));
    Statement stmt=con.createStatement("select * from streams where fileid=?");
    stmt.bind(1,fileid);
    ResultSet rs=stmt.executeQuery();
//    int cc=rs.getColumnCount();
    while(rs.next()){
//	Stream::edit_stream(arg);
    }
}
