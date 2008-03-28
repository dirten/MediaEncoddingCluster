#include "Files.h"
#include "Page.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/StringUtil.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include "Stream.h"
#include <iostream>
#include "import.cpp"
using namespace org::esb::web;
using namespace org::esb::sql;
using namespace org::esb::config;
//using namespace org::esb::utils;
//char  value[100];
bool isFirst=true;
//string str;
void Files::upload_file(struct shttpd_arg *arg){

	const char	*s;
	char *path;
	struct state {
		size_t	cl;		/* Content-Length	*/
		size_t	nread;		/* Number of bytes read	*/
		FILE	*fp;
		char	*filename;
	} *state;

	/* If the connection was broken prematurely, cleanup */
	if (arg->flags & SHTTPD_CONNECTION_ERROR && arg->state) {
		(void) fclose(((struct state *) arg->state)->fp);
		free(arg->state);
		isFirst=true;

	} else if (isFirst&&(s = shttpd_get_header(arg, "Content-Length")) == NULL) {
		shttpd_printf(arg, "HTTP/1.0 411 Length Required\n\n");
		arg->flags |= SHTTPD_END_OF_OUTPUT;
		isFirst=true;
	} else if (arg->state == NULL) {
		/* New request. Allocate a state structure, and open a file */
		arg->state = state = (struct state*)calloc(1, sizeof(*state));
		state->cl = strtoul(s, NULL, 10);
		string str(arg->in.buf,arg->in.len);
		StringTokenizer tok(str,"\r\n");
		for(int a=0;a<3;a++){
		    string line=tok.nextToken();
		    if(a==1){
		        StringTokenizer tokLine(line,";");
		        tokLine.nextToken();
		        tokLine.nextToken();
		        string filename= tokLine.nextToken();
		        filename=filename.substr(11);
		        filename=filename.substr(0,filename.length()-1);
			state->filename = new char[filename.length()+1];
//			state->filename =(char *)filename.c_str();
			memset(state->filename,0,filename.length()+1);
			memcpy(state->filename,filename.c_str(),filename.length());
			if(strlen(state->filename)==0){
			    shttpd_printf(arg, "HTTP/1.0 200 OK\n"
				"Content-Type: text/plain\n\n");
			    arg->flags |= SHTTPD_END_OF_OUTPUT;
			}
			state->fp = fopen(state->filename, "wb+");
		        cout << state->filename<< endl;
		    }
		}
		shttpd_printf(arg, "HTTP/1.0 200 OK\n"
			"Content-Type: text/plain\n\n");
	} else	{
		/*cut out Post Header*/
		char * buffer;
		int len=0,buffer_len=0;
		
		string str(arg->in.buf,arg->in.len);
		if(isFirst){
			StringTokenizer tok(str,"\r\n");
			for(int a=0;a<3;a++){
				string line=tok.nextToken();
				len+=line.length()+2;
			}
			str=str.substr(len+2);
			buffer=(char*)str.c_str();
			buffer_len=str.length();
			isFirst=false;
		}else{
			buffer=arg->in.buf;
			buffer_len=arg->in.len;
		}
		state = (struct state*)arg->state;
		(void) fwrite(buffer, buffer_len, 1, state->fp);
		state->nread += arg->in.len;
		arg->in.num_bytes = arg->in.len;
		if (state->nread >= state->cl) {
			shttpd_printf(arg, "Written %d bytes to %s from ",
			    state->nread, state->filename);
			(void) fclose(state->fp);
			char * argv[]={"",state->filename};			
			import(2,argv);
			free(state);
			arg->flags |= SHTTPD_END_OF_OUTPUT;
			isFirst=true;
		}
	}
}

void Files::show_files(struct shttpd_arg *arg){
    Connection con(Config::getProperty("db.connection"));
    Statement stmt=con.createStatement("select * from files where type=1");
    ResultSet rs=stmt.executeQuery();
    shttpd_printf(arg, "<table class=\"list\">");
    shttpd_printf(arg, "<tr class=\"header\"><td>Id</td><td>Filename</td><td>Size</td><td>Insert Date</td></tr>");
    while(rs.next()){
        shttpd_printf(arg, "<tr class=\"row\"><td>%d.</td><td><a href=\"filedetails.shtml?file=%d\">%s</a></td><td> %.02dMB</td><td>%s</td></tr>",rs.getint(0),rs.getint(0),rs.getstring(1).c_str(),(rs.getint(4)/1024/1024),rs.getstring(2).c_str());
    }
    shttpd_printf(arg, "</table>");
    arg->flags |= SHTTPD_END_OF_OUTPUT;
}

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
    
    shttpd_printf(arg, "<div>");
    
    shttpd_printf(arg, "<li>Upload file example. "
	    "<form action=\"/upload\" method=\"post\" enctype=\"multipart/form-data\" "
	    "action=\"/post\"><input type=\"file\" name=\"file\">"
	    "<input type=\"submit\"></form>");

    shttpd_printf(arg, "</div>");
    shttpd_printf(arg, "</div>");
    }
}

void Files::show_details(struct shttpd_arg *arg, Properties & props){
    int id=atoi(props.getProperty("filedetails"));
    Connection con(Config::getProperty("db.connection"));
    {
	Statement stmt=con.createStatement("select id from streams where fileid=? order by stream_index");
	stmt.bind(1,id);
        ResultSet rs=stmt.executeQuery();
        while(rs.next()){
    	    props.setProperty("streamid",rs.getstring(0));
    	    Stream::show_input_stream(arg, props);
		}
    }
    {
	Statement stmt=con.createStatement("select s.id from jobs j, job_details jd, streams s where infile=? and j.id=jd.job_id and jd.outstream=s.id order by s.stream_type;");
	stmt.bind(1,id);
        ResultSet rs=stmt.executeQuery();
        while(rs.next()){
    	    props.setProperty("streamid",rs.getstring(0));
    	    Stream::show_output_stream(arg, props);
		}
    }
    shttpd_printf(arg, "<div>");
    shttpd_printf(arg, "<a href=\"?fileid=%d&page=streams&edit_stream=-1\">Add Stream</a>", id);
    shttpd_printf(arg, "</div>");

}

void Files::show_details(struct shttpd_arg *arg){
    Properties props=Page::getParams(arg);
    int id=0;
    if(props.hasProperty("file"))
	id=atoi(props.getProperty("file"));
    
    Connection con(Config::getProperty("db.connection"));
    {
	Statement stmt=con.createStatement("select id from streams where fileid=? order by stream_index");
	stmt.bind(1,id);
        ResultSet rs=stmt.executeQuery();
        while(rs.next()){
    	    props.setProperty("streamid",rs.getstring(0));
    	    Stream::show_input_stream(arg, props);
	}
    }
    {
	Statement stmt=con.createStatement("select s.id from jobs j, job_details jd, streams s where infile=? and j.id=jd.job_id and jd.outstream=s.id order by s.stream_type;");
	stmt.bind(1,id);
        ResultSet rs=stmt.executeQuery();
        while(rs.next()){
    	    props.setProperty("streamid",rs.getstring(0));
    	    Stream::show_output_stream(arg, props);
	}
    }
//    shttpd_printf(arg, "<div>");
//    shttpd_printf(arg, "<a href=\"?fileid=%d&page=streams&edit_stream=-1\">Add Stream</a>", id);
//    shttpd_printf(arg, "</div>");
    arg->flags |= SHTTPD_END_OF_OUTPUT;
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
