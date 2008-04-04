#ifndef IMPORT_CPP
#define IMPORT_CPP
//#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include "CreateDatabase.cpp"
//#include <avformat.h>
#include <boost/progress.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/polymorphic_binary_iarchive.hpp> 
//#include <boost/archive/polymorphic_binary_oarchive.hpp> 


#include "org/esb/config/config.h"
#include "tntdb/connect.h"
#include "tntdb/connection.h"
#include "tntdb/statement.h"
#include "tntdb/blob.h"
//#include "org/esb/sql/sqlite3x.hpp"
using namespace std;
using namespace org::esb::io;
using namespace org::esb::config;
using namespace tntdb;
using namespace org::esb::av;
using namespace boost;
//using namespace sqlite3x;



int import(int argc, char * argv[]){
	cout << LIBAVCODEC_IDENT <<endl;

    Config::init("./cluster.cfg");

	if(argc!=2){
		cout << "wrong parameter count"<<endl;	
		exit(1);
	}
	string dbFile=Config::getProperty("data.dir");
	dbFile+="/";
	dbFile+=Config::getProperty("data.file");
//	cout << dbFile;
	File databaseFile(dbFile.c_str());
//	File databaseFile("/tmp/hive.db3");
	if(!databaseFile.exists()||!checkDatabase(databaseFile)){
		createDatabase(databaseFile);
	}else{
	    cout << "Database not found";
	}

	File inputFile(argv[1]);
	if(!inputFile.canRead()){
		cout << "Source File not found"<<endl;
	}else{
		cout << "File:"<<inputFile.getPath()<<endl;
	}

    

	int fileid=0,count=0, frame_group=0;

	FormatInputStream fis(&inputFile);
	PacketInputStream pis(&fis);

	
	tntdb::Connection con=connect(databaseFile.getPath());
//	sqlite3_transaction trans=con.getTransaction();
	{
	    tntdb::Statement st=con.prepare("INSERT INTO files(filename, size, type, insertdate,stream_count, title,author, copyright, comment, album, year, track, genre, duration,bitrate) values "
	    "(:filename, :size,1,datetime('now'),:stream_count, :title, :author, :copyright, :comment, :album, :year, :track, :genre, :duration, :bitrate)");
	    st.set("filename",inputFile.getPath());
	    st.set("size",(double )fis.getFileSize());
	    st.set("stream_count",(double )fis.getStreamCount());
	    st.set("title",(char*)&fis.getFormatContext()->title);
	    st.set("author",(char*)&fis.getFormatContext()->author);
	    st.set("copyright",(char*)&fis.getFormatContext()->copyright);
	    st.set("comment",(char*)&fis.getFormatContext()->comment);
	    st.set("album",(char*)&fis.getFormatContext()->album);
	    st.set("year",(char*)&fis.getFormatContext()->year);
	    st.set("track",(char*)&fis.getFormatContext()->track);
	    st.set("genre",(char*)&fis.getFormatContext()->genre);
	    st.set("duration",(unsigned int)&fis.getFormatContext()->duration);
	    st.set("bitrate",&fis.getFormatContext()->bit_rate);
	    st.execute();
	}
//	con.executenonquery(string("INSERT INTO files(filename,size) values ( '")+inputFile.getPath()+string("',")+fis.getFileSize()+")");
//	fileid=con.lastInsertId();

	AVFormatContext * ctx=fis.getFormatContext();

	int streams[ctx->nb_streams];
	long duration=0;
	tntdb::Statement stmt_str=con.prepare( "insert into streams (fileid,stream_index, stream_type,codec, codec_name,framerate,start_time,duration,time_base_num, time_base_den, width, height, gop_size, pix_fmt,bit_rate, rate_emu, sample_rate, channels, sample_fmt) values "
	"(:fileid, :stream_index, :stream_type, :codec, :codec_name, :framerate, :start_time, :duration, :time_base_num, :time_base_den, :width, :height, :gop_size, :pix_fmt, :bit_rate, :rate_emu, :sample_rate, :channels, :sample_fmt)" );
	for(unsigned int a =0;a<ctx->nb_streams;a++){
	    int field=1;
		duration+=ctx->streams[a]->duration;
	    stmt_str.set( "fileid", fileid);
	    stmt_str.set( "stream_index", (int)a);
	    stmt_str.set( "stream_type", ctx->streams[a]->codec->codec_type);
	    stmt_str.set( "codec", ctx->streams[a]->codec->codec_id);
	    stmt_str.set( "code_name", ctx->streams[a]->codec->codec_name);
	    stmt_str.set( "framerate", av_q2d(ctx->streams[a]->r_frame_rate));
	    stmt_str.set( "start_time", (double)ctx->streams[a]->start_time);
	    stmt_str.set( "duration", (double)ctx->streams[a]->duration);
	    stmt_str.set( "time_base_num", ctx->streams[a]->time_base.num);
	    stmt_str.set( "time_base_den", ctx->streams[a]->time_base.den);
	    stmt_str.set( "width", ctx->streams[a]->codec->width);
	    stmt_str.set( "height", ctx->streams[a]->codec->height);
	    stmt_str.set( "gop_size", ctx->streams[a]->codec->gop_size);
	    stmt_str.set( "pix_fmt", ctx->streams[a]->codec->pix_fmt);
	    stmt_str.set( "bit_rate", ctx->streams[a]->codec->bit_rate);
	    stmt_str.set( "rate_emu", ctx->streams[a]->codec->rate_emu);
	    stmt_str.set( "sample_rate", ctx->streams[a]->codec->sample_rate);
	    stmt_str.set( "channels", ctx->streams[a]->codec->channels);
	    stmt_str.set( "sample_fmt", ctx->streams[a]->codec->sample_fmt);
	    stmt_str.execute();
   	    int streamid =0;//con.lastInsertId();
   	    streams[a]=streamid;
        }
	progress_display show_progress(duration);

    Packet packet;

    tntdb::Statement stmt=con.prepare("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values "
    "(NULL,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data)");
    while(true/*&&count < 1000*/){

        pis.readPacket(packet);

        if(packet.data==NULL)break;
        
        ++count;

	if(packet.stream_index==0&&packet.isKeyFrame())frame_group++;
	int  field=1;
	packet.duration=packet.duration==0?1:packet.duration;
        stmt.set( "stream_id", streams[packet.stream_index]);
        stmt.set( "pts", (double)packet.pts);
        stmt.set( "dts", (double)packet.dts);
        stmt.set( "stream_index", packet.stream_index);
        stmt.set( "key_frame", packet.isKeyFrame());
	if(packet.stream_index==0)
    	    stmt.set( "frame_group", frame_group);
	else
    	    stmt.set( "frame_group",0);	
        stmt.set( "flags", packet.flags);
        stmt.set( "duration", packet.duration);
        stmt.set( "pos", (double)packet.pos);
        stmt.set( "data_size", packet.size);
        Blob blob((const char*)packet.data,packet.size);
        stmt.set( "data", blob);
	stmt.execute();
	show_progress+=packet.duration;

    }
    
//    trans.commit();
    cout << endl;
//	show_progress=duration;
    	return 0;
}
/*
int main(int argc, char * argv[]){
	import(argc,argv);
}
*/
#endif

