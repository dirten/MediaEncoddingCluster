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
//#include "tntdb/connect.h"
//#include "tntdb/connection.h"
//#include "tntdb/statement.h"
//#include "tntdb/blob.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
//#include "org/esb/sql/sqlite3x.hpp"
using namespace std;
using namespace org::esb::io;
using namespace org::esb::sql;
using namespace org::esb::config;
//using namespace tntdb;
using namespace org::esb::av;
using namespace boost;
//using namespace sqlite3x;



int import(int argc, char * argv[]){
	cout << LIBAVCODEC_IDENT <<endl;


	if(argc!=2){
		cout << "wrong parameter count"<<endl;	
		exit(1);
	}
//	Config::init("./cluster.cfg");
	
	
	string connect_str=Config::getProperty("db.connection");
	Connection con(connect_str.c_str());
	if(!checkDatabase(con)){
		createDatabase(con);
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

	
//	tntdb::Connection con=connect(databaseFile.getPath());
//	sqlite3_transaction trans=con.getTransaction();
	{
	    PreparedStatement st=con.prepareStatement("INSERT INTO files(filename, size, type, insertdate,stream_count, title,author, copyright, comment, album, year, track, genre, duration,bitrate) values "
	    "(?,?,1,now(),?,?,?,?,?,?,?,?,?,?,?)");
//	    "(:filename, :size,1,datetime('now'),:stream_count, :title, :author, :copyright, :comment, :album, :year, :track, :genre, :duration, :bitrate)");
	    int field=0;
	    string tmp="testfile.test";
	    st.setString(field++,(char*)inputFile.getPath());
	    st.setDouble(field++,(double )fis.getFileSize());
	    st.setDouble(field++,(double )fis.getStreamCount());
	    st.setString(field++,fis.getFormatContext()->title);
	    st.setString(field++,(char*)fis.getFormatContext()->author);
	    st.setString(field++,(char*)fis.getFormatContext()->copyright);
	    st.setString(field++,(char*)fis.getFormatContext()->comment);
	    st.setString(field++,(char*)fis.getFormatContext()->album);
	    st.setInt(field++,fis.getFormatContext()->year);
	    st.setString(field++,(char*)fis.getFormatContext()->track);
	    st.setString(field++,(char*)fis.getFormatContext()->genre);
	    st.setInt(field++,(unsigned int)fis.getFormatContext()->duration);
	    st.setInt(field++,(unsigned int)fis.getFormatContext()->bit_rate);
	    st.execute();
	    fileid=st.getLastInsertId();
	}
//	con.executenonquery(string("INSERT INTO files(filename,size) values ( '")+inputFile.getPath()+string("',")+fis.getFileSize()+")");

	AVFormatContext * ctx=fis.getFormatContext();

	int streams[ctx->nb_streams];
	long duration=0;
	PreparedStatement stmt_str=con.prepareStatement( "insert into streams (fileid,stream_index, stream_type,codec, codec_name,framerate,start_time,duration,time_base_num, time_base_den, width, height, gop_size, pix_fmt,bit_rate, rate_emu, sample_rate, channels, sample_fmt) values "
	"(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
//	"(:fileid, :stream_index, :stream_type, :codec, :codec_name, :framerate, :start_time, :duration, :time_base_num, :time_base_den, :width, :height, :gop_size, :pix_fmt, :bit_rate, :rate_emu, :sample_rate, :channels, :sample_fmt)" );
	for(unsigned int a =0;a<ctx->nb_streams;a++){
	    int field=0;
	    duration+=ctx->streams[a]->duration;
	    stmt_str.setInt(field++, fileid);
	    stmt_str.setInt( field++, (int)a);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->codec_type);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->codec_id);
	    string tmp=ctx->streams[a]->codec->codec_name;
	    stmt_str.setString( field++, tmp);
	    stmt_str.setDouble( field++, av_q2d(ctx->streams[a]->r_frame_rate));
	    stmt_str.setDouble( field++, (double)ctx->streams[a]->start_time);
	    stmt_str.setDouble( field++, (double)ctx->streams[a]->duration);
	    stmt_str.setInt( field++, ctx->streams[a]->time_base.num);
	    stmt_str.setInt( field++, ctx->streams[a]->time_base.den);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->width);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->height);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->gop_size);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->pix_fmt);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->bit_rate);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->rate_emu);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->sample_rate);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->channels);
	    stmt_str.setInt( field++, ctx->streams[a]->codec->sample_fmt);
	    stmt_str.execute();
   	    int streamid =stmt_str.getLastInsertId();
   	    streams[a]=streamid;
        }
	progress_display show_progress(duration);

    Packet packet;

    PreparedStatement stmt=con.prepareStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values "
    "(NULL,?,?,?,?,?,?,?,?,?,?,?)");
//    "(NULL,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data)");
    while(true/*&&count < 1000*/){

        pis.readPacket(packet);

        if(packet.data==NULL)break;
        
        ++count;

	if(packet.stream_index==0&&packet.isKeyFrame())frame_group++;
	int  field=0;
	packet.duration=packet.duration==0?1:packet.duration;
        stmt.setInt( field++, streams[packet.stream_index]);
        stmt.setDouble( field++, (double)packet.pts);
        stmt.setDouble( field++, (double)packet.dts);
        stmt.setInt( field++, packet.stream_index);
        stmt.setInt( field++, packet.isKeyFrame());
	if(packet.stream_index==0)
    	    stmt.setInt( field++, frame_group);
	else
    	    stmt.setInt( field++,0);	
        stmt.setInt( field++, packet.flags);
        stmt.setInt( field++, packet.duration);
        stmt.setDouble( field++, (double)packet.pos);
        stmt.setInt( field++, packet.size);
//        Blob blob((const char*)packet.data,packet.size);
        stmt.setBlob( field++, (char*)packet.data, packet.size);
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

