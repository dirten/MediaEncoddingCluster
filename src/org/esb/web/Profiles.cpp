#include "Profiles.h"
#include "Page.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/av/Codec.h"
#include "org/esb/config/config.h"
using namespace org::esb::web;
using namespace org::esb::sql;
using namespace org::esb::config;
using namespace org::esb::av;

void Profiles::list(struct shttpd_arg *arg){
    Connection con(Config::getProperty("db.connection"));
    Statement stmt=con.createStatement("select * from profiles");
    ResultSet rs=stmt.executeQuery();
    shttpd_printf(arg, "<table class=\"list\" cellspacing=\"0\" cellpadding=\"0\">");
    shttpd_printf(arg, "<tr class=\"header\"><td>Id</td><td>Profile Name</td><td>Format</td><td>Video Codec</td><td>Video Bitrate</td><td>Video Framerate</td></tr>");

    while(rs.next()){
	shttpd_printf(arg, "<tr class=\"row\"><td>%d.</td><td><a href=\"profiles_edit.shtml?profileid=%d\">%s</a></td>",rs.getInt("id"),rs.getInt("id"),rs.getString("profile_name").c_str());
	shttpd_printf(arg, "<td>%s</td>",rs.getString("v_format").c_str());
	shttpd_printf(arg, "<td>%s</td>",rs.getString("v_codec").c_str());
	shttpd_printf(arg, "<td>%s</td>",rs.getString("v_bitrate").c_str());
	shttpd_printf(arg, "<td>%s</td>",rs.getString("v_framerate").c_str());
	shttpd_printf(arg, "</tr>");
    }
    shttpd_printf(arg, "<a href=\"profiles_edit.shtml?profileid=-1\">Create new Profile</a>");
    shttpd_printf(arg, "</table>");
    arg->flags |= SHTTPD_END_OF_OUTPUT;

}

void Profiles::edit(struct shttpd_arg *arg){

    Properties props=Page::getParams(arg);
    int id=-1;
    if(props.hasProperty("profileid"))
	id=atoi(props.getProperty("profileid"));

    Connection con(Config::getProperty("db.connection"));
    Statement stmt=con.createStatement("select * from profiles where id=?");
    stmt.bind(1,id);
    ResultSet rs=stmt.executeQuery();
    bool data=rs.next();
    shttpd_printf(arg, "<form name=\"save_profile\" action=\"profiles_save\" method=\"post\">");
    shttpd_printf(arg, "<table class=\"list\" cellspacing=\"0\" cellpadding=\"0\">");

    shttpd_printf(arg, "<tr><td>Profile Name:</td><td><input type=\"text\" name=\"profile_name\" value=\"%s\"></td></tr>",data?rs.getString("profile_name").c_str():"");
//    shttpd_printf(arg, "<tr><td>Profile Name:</td><td><input type=\"text\" name=\"profile_name\" value=\"%s\"></td></tr>",data?rs.getString("profile_name").c_str():"");

	AVOutputFormat *f=NULL;
	shttpd_printf(arg, "<tr><td>Format:</td><td><select name=\"v_format\">");
	while((f= av_oformat_next(f))) {
		string selected="";
		if(strcmp(f->name,rs.getString("v_format").c_str())==0){
		    selected="selected";
		}
//		if(f->encode&&p->type==CODEC_TYPE_VIDEO)
			shttpd_printf(arg, "<option %s value=\"%s\">%s</option>",selected.c_str(),f->name, f->long_name);	
	}
	shttpd_printf(arg, "</select></td></tr>");

	AVCodec *p=NULL;
	shttpd_printf(arg, "<tr><td>Video Codec:</td><td><select name=\"v_codec\">");
	while((p= av_codec_next(p))) {
		string selected="";
		if(p->id==rs.getInt("v_codec")){
		    selected="selected";
		}
		if(p->encode&&p->type==CODEC_TYPE_VIDEO)
			shttpd_printf(arg, "<option %s value=\"%d\">%s</option>",selected.c_str(),p->id, p->name);	
	}
	shttpd_printf(arg, "</select></td></tr>");

    shttpd_printf(arg, "");
    shttpd_printf(arg, "<tr><td>Bit Rate:</td><td><input type=\"text\" name=\"v_bitrate\" value=\"%.2f\"></td></tr>",data?atof(rs.getString("v_bitrate").c_str()):300.0);
    shttpd_printf(arg, "<tr><td>Frame Rate:</td><td><input type=\"text\" name=\"v_framerate\" value=\"%.3f\"></td></tr>",data?atof(rs.getString("v_framerate").c_str()):25.0);
    shttpd_printf(arg, "<tr><td>Frame Width:</td><td><input type=\"text\" name=\"v_width\" value=\"%d\"></td></tr>",data?atoi(rs.getString("v_width").c_str()):0);
    shttpd_printf(arg, "<tr><td>Frame height:</td><td><input type=\"text\" name=\"v_height\" value=\"%d\"></td></tr>",data?atoi(rs.getString("v_height").c_str()):0);
    shttpd_printf(arg, "<tr><td>Audio Channels:</td><td><input type=\"text\" name=\"a_channels\" value=\"%d\"></td></tr>",data?atoi(rs.getString("a_channels").c_str()):2);

	AVCodec *ac=NULL;
	shttpd_printf(arg, "<tr><td>Audio Codec:</td><td><select name=\"a_codec\">");
	while((ac= av_codec_next(ac))) {
		string selected="";
		if(ac->id==rs.getInt("a_codec")){
		    selected="selected";
		}
		if(ac->encode&&ac->type==CODEC_TYPE_AUDIO)
			shttpd_printf(arg, "<option %s value=\"%d\">%s</option>",selected.c_str(),ac->id, ac->name);	
	}
	shttpd_printf(arg, "</select></td></tr>");
    shttpd_printf(arg, "<tr><td>Audio Bitrate:</td><td><input type=\"text\" name=\"a_bitrate\" value=\"%d\"></td></tr>",data?rs.getInt("a_bitrate"):128000);
    /*@TODO rs.getInt(11) austauschen durch qualifizierten namen im ResultSet. z.Z. ist das ResultSet noch Buggy, der Wert wird nicht über den Namen aufgelöst*/
    shttpd_printf(arg, "<tr><td>Audio Samplerate:</td><td><input type=\"text\" name=\"a_samplerate\" value=\"%d\"></td></tr>",data?rs.getInt(11):44100);

    shttpd_printf(arg, "<input type=\"hidden\" name=\"profileid\" value=\"%d\">",id);
//    shttpd_printf(arg, "<input type=\"hidden\" name=\"outstreamids\" value=\"%s\">",streamids.c_str());
    shttpd_printf(arg, "<tr><td><input type=\"submit\" value=\"save\"></td></tr>");
    shttpd_printf(arg, "</form>");
    shttpd_printf(arg, "</table>");
    arg->flags |= SHTTPD_END_OF_OUTPUT;
}

void Profiles::save(struct shttpd_arg *arg){
    char idchar[10];
    (void) shttpd_get_var("profileid", arg->in.buf, arg->in.len, idchar, sizeof(idchar));
    int id=atoi(idchar);
    string sql="";
    if(id==-1){
	sql.append("INSERT INTO profiles (profile_name, v_format, v_codec, v_bitrate,v_framerate,v_width, v_height, a_channels, a_codec, a_bitrate,a_samplerate) values(?,?,?,?,?,?,?,?,?,?,?)");
//	sql.append("INSERT INTO profiles (profile_name) values(?)");
    }else{
	sql.append("UPDATE profiles SET profile_name=?, v_format=?, v_codec=?, v_bitrate=?,v_framerate=?,v_width=?, v_height=?, a_channels=?, a_codec=?, a_bitrate=?,a_samplerate=? where id=?");
//	sql.append("UPDATE profiles SET profile_name=? where id=?");
    }
//    cout << "Profile:"<<sql<<endl;
    Connection con(Config::getProperty("db.connection"));
    Statement stmt=con.createStatement(sql.c_str());

    char profile_name[255];
    (void) shttpd_get_var("profile_name", arg->in.buf, arg->in.len, profile_name, sizeof(profile_name));
    int field=1;
    stmt.bind(field++,profile_name);

    char v_format[10];
    (void) shttpd_get_var("v_format", arg->in.buf, arg->in.len, v_format, sizeof(v_format));
    stmt.bind(field++,v_format);

    char v_codec[10];
    (void) shttpd_get_var("v_codec", arg->in.buf, arg->in.len, v_codec, sizeof(v_codec));
    stmt.bind(field++,v_codec);

    char v_bitrate[10];
    (void) shttpd_get_var("v_bitrate", arg->in.buf, arg->in.len, v_bitrate, sizeof(v_bitrate));
    stmt.bind(field++,v_bitrate);

    char v_framerate[10];
    (void) shttpd_get_var("v_framerate", arg->in.buf, arg->in.len, v_framerate, sizeof(v_framerate));
    stmt.bind(field++,v_framerate);

    char v_width[10];
    (void) shttpd_get_var("v_width", arg->in.buf, arg->in.len, v_width, sizeof(v_width));
    stmt.bind(field++,v_width);

    char v_height[10];
    (void) shttpd_get_var("v_height", arg->in.buf, arg->in.len, v_height, sizeof(v_height));
    stmt.bind(field++,v_height);

    char a_channels[10];
    (void) shttpd_get_var("a_channels", arg->in.buf, arg->in.len, a_channels, sizeof(a_channels));
    stmt.bind(field++,a_channels);

    char a_codec[10];
    (void) shttpd_get_var("a_codec", arg->in.buf, arg->in.len, a_codec, sizeof(a_codec));
    stmt.bind(field++,a_codec);

    char a_bitrate[10];
    (void) shttpd_get_var("a_bitrate", arg->in.buf, arg->in.len, a_bitrate, sizeof(a_bitrate));
    stmt.bind(field++,a_bitrate);

    char a_samplerate[10];
    (void) shttpd_get_var("a_samplerate", arg->in.buf, arg->in.len, a_samplerate, sizeof(a_samplerate));
    stmt.bind(field++,a_samplerate);

    if(id>0){
        stmt.bind(field++,id);    
    }
    
    stmt.execute();
    shttpd_printf(arg, "HTTP/1.1 303 See Other\r\n"
			"Location: %s\r\n\r\n", "profiles.shtml");
    arg->flags |= SHTTPD_END_OF_OUTPUT;

}
void Profiles::selector(struct shttpd_arg *arg){
    Connection con(Config::getProperty("db.connection"));
    Statement stmt=con.createStatement("select id,profile_name from profiles");
    ResultSet rs=stmt.executeQuery();
    shttpd_printf(arg, "<select name=\"profile\">");
    while(rs.next()){
	shttpd_printf(arg, "<option value=\"%d\">%s</option>",rs.getInt(0),rs.getString(1).c_str());
    }
    shttpd_printf(arg, "</select>");
    arg->flags |= SHTTPD_END_OF_OUTPUT;

}

