#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include "org/esb/av/Codec.h"
#include "Stream.h"
#include "WTK.h"
//#include "avformat.h"




using namespace org::esb::web;
using namespace org::esb::sql;
using namespace org::esb::av;
using namespace org::esb::config;

void Stream::show_input_stream(struct shttpd_arg *arg, Properties & props){
    if(props.hasProperty("streamid")){
    Connection con(Config::getProperty("db.connection"));
    Statement stmt=con.createStatement("select * from streams where id=?");
    stmt.bind(1,atoi(props.getProperty("streamid")));
    ResultSet rs=stmt.executeQuery();
//    int cc=rs.getColumnCount();
    while(rs.next()){
	shttpd_printf(arg, "<div class=\"stream_info_block\">");
	shttpd_printf(arg, "<div>Stream ( %s )</div>",rs.getstring(2).c_str());
	Codec codec((CodecID)rs.getint(4));
	if(codec.codec_type==CODEC_TYPE_AUDIO){
	    codec.setSampleRate(1);
	    codec.setChannels(1);
	    codec.setBitRate(100);
	}
	
	codec.open();
	shttpd_printf(arg, "<div>Type: %s</div>",codec.codec_type==CODEC_TYPE_AUDIO?"Audio":"Video");
	shttpd_printf(arg, "<div>CodecName: %s</div>",codec.codec->name);
	shttpd_printf(arg, "<div>Time Base: %d/%d</div>",rs.getint(9),rs.getint(10));	
	shttpd_printf(arg, "<div>Duration: %00.00f sec.</div>",rs.getdouble(8)/(rs.getdouble(10)/rs.getdouble(9)));
	shttpd_printf(arg, "<div>Bit Rate: %d </div>",rs.getint(16));
	if(codec.codec_type==CODEC_TYPE_VIDEO){
	    shttpd_printf(arg, "<div>Frame Rate: %d</div>",rs.getint(6));	
	    shttpd_printf(arg, "<div>Dimension WxH: %dx%d</div>",rs.getint(12),rs.getint(13));
	}else
	if(codec.codec_type==CODEC_TYPE_AUDIO){
	    shttpd_printf(arg, "<div>Sample Rate: %d</div>",rs.getint(18));	
	    shttpd_printf(arg, "<div>Channels: %d</div>",rs.getint(19));	
	}
	shttpd_printf(arg, "</div>");
    }
    }else{
	shttpd_printf(arg, "no stream id");
    }
}

void Stream::show_output_stream(struct shttpd_arg *arg, Properties & props){

    Connection con(Config::getProperty("db.connection"));
    Statement stmt=con.createStatement("select * from streams where id=?");
    stmt.bind(1,atoi(props.getProperty("streamid")));
    ResultSet rs=stmt.executeQuery();

    if(rs.next()){
	shttpd_printf(arg, "<div class=\"stream_info_block\">");
	shttpd_printf(arg, "<div>OutputStream ( %s )</div>",rs.getstring(2).c_str());
	Codec codec((CodecID)rs.getint(4));
	codec.open();
	AVCodec *p=NULL;
	shttpd_printf(arg, "<div>Type: %s</div>",codec.codec_type==CODEC_TYPE_AUDIO?"Audio":"Video");

	shttpd_printf(arg, "<div>Codec:<select name=\"codec\">");
	while((p= av_codec_next(p))) {
		string selected="";
		if(strcmp(p->name,codec.codec->name)==0){
		    selected="selected";
		}
		if(p->encode&&p->type==codec.codec->type)
			shttpd_printf(arg, "<option %s>%s</option>",selected.c_str(), p->name);	
	}
	shttpd_printf(arg, "</select></div>");
	shttpd_printf(arg, "<div>Time Base: %d/%d</div>",rs.getint(9),rs.getint(10));	
	shttpd_printf(arg, "<div>Duration: %00.00f sec.</div>",rs.getdouble(8)/rs.getdouble(10));
	shttpd_printf(arg, "<div>Bit Rate: <input type=\"text\" value=\"%d\"> </div>",rs.getint(16));
	if(codec.codec_type==CODEC_TYPE_VIDEO){
	    shttpd_printf(arg, "<div>Frame Rate: %d</div>",rs.getint(6));	
	    shttpd_printf(arg, "<div>Dimension WxH: %dx%d</div>",rs.getint(12),rs.getint(13));
	}else
	if(codec.codec_type==CODEC_TYPE_AUDIO){
	    shttpd_printf(arg, "<div>Sample Rate: <input type=\"text\" value=\"%d\"></div>",rs.getint(18));	
	    shttpd_printf(arg, "<div>Channels: <input type=\"text\" value=\"%d\"></div>",rs.getint(19));	
	}
	shttpd_printf(arg, "</div>");
    }
}

void Stream::save_output_stream(struct shttpd_arg *arg){
	char value[100];
	(void) shttpd_get_var("test", arg->in.buf, arg->in.len, value, sizeof(value));
	cout << "save stream details:codec:"<<value<<endl;
	shttpd_printf(arg, "HTTP/1.1 303 See Other\r\n"
	 				"Location: %s\r\n\r\n", "index.shtml");
	    arg->flags |= SHTTPD_END_OF_OUTPUT;
}
