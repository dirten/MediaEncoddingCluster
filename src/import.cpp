#ifndef IMPORT_CPP
#define IMPORT_CPP
//#include <iostream>
//#include <fstream>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
//#include "CreateDatabase.cpp"
//#include <boost/progress.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
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


template <class T>
    T nullCheck(T param){
	return param;
    }

int import(int argc, char * argv[]){
	cout << LIBAVCODEC_IDENT <<endl;


	if(argc!=2){
		cout << "wrong parameter count"<<endl;	
		exit(1);
	}
//	Config::init("./cluster.cfg");
	
	
	string connect_str=Config::getProperty("db.connection");
	Connection con(connect_str.c_str());
	/*
	if(!checkDatabase(con)){
		createDatabase(con);
	}else{
	    cout << "Database not found";
	}
*/
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
//	    "(?,?,1,now(),?,?,?,?,?,?,?,?,?,?,?)");
	    "(:filename, :size,1,now(),:stream_count, :title, :author, :copyright, :comment, :album, :year, :track, :genre, :duration, :bitrate)");
	    int field=0;
	    string tmp="testfile.test";
	    st.setString("filename",nullCheck<char*>((char*)inputFile.getPath()));
	    st.setDouble("size",(double )fis.getFileSize());
	    st.setDouble("stream_count",(double )fis.getStreamCount());
	    st.setString("title",fis.getFormatContext()->title);
	    st.setString("author",(char*)fis.getFormatContext()->author);
	    st.setString("copyright",(char*)fis.getFormatContext()->copyright);
	    st.setString("comment",(char*)fis.getFormatContext()->comment);
	    st.setString("album",(char*)fis.getFormatContext()->album);
	    st.setInt("year",fis.getFormatContext()->year);
	    st.setInt("track",(unsigned int)fis.getFormatContext()->track);
	    st.setString("genre",(char*)fis.getFormatContext()->genre);
	    st.setInt("duration",(unsigned int)fis.getFormatContext()->duration);
	    st.setInt("bitrate",(unsigned int)fis.getFormatContext()->bit_rate);
	    st.execute();
	    fileid=con.lastInsertId();
	}
//	con.executenonquery(string("INSERT INTO files(filename,size) values ( '")+inputFile.getPath()+string("',")+fis.getFileSize()+")");

	AVFormatContext * ctx=fis.getFormatContext();

	int streams[ctx->nb_streams];
	long duration=0;
	PreparedStatement stmt_str=con.prepareStatement( "insert into streams (fileid,stream_index, stream_type,codec, codec_name,framerate,start_time,duration,time_base_num, time_base_den, width, height, gop_size, pix_fmt,bit_rate, rate_emu, sample_rate, channels, sample_fmt, priv_data_size, priv_data) values "
//	"(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
	"(:fileid, :stream_index, :stream_type, :codec, :codec_name, :framerate, :start_time, :duration, :time_base_num, :time_base_den, :width, :height, :gop_size, :pix_fmt, :bit_rate, :rate_emu, :sample_rate, :channels, :sample_fmt, :priv_data_size, :priv_data)" );
	for(unsigned int a =0;a<ctx->nb_streams;a++){
	    int field=0;
	    duration+=ctx->streams[a]->duration;
	    stmt_str.setInt("fileid", fileid);
	    stmt_str.setInt( "stream_index", (int)a);
	    stmt_str.setInt( "stream_type", ctx->streams[a]->codec->codec_type);
	    stmt_str.setInt( "codec", ctx->streams[a]->codec->codec_id);
//	    string tmp=ctx->streams[a]->codec->codec_name;
	    stmt_str.setString( "codec_name", ctx->streams[a]->codec->codec_name);
	    stmt_str.setDouble( "framerate", av_q2d(ctx->streams[a]->r_frame_rate));
	    stmt_str.setDouble( "start_time", (double)ctx->streams[a]->start_time);
	    stmt_str.setDouble( "duration", (double)ctx->streams[a]->duration);
	    stmt_str.setInt( "time_base_num", ctx->streams[a]->time_base.num);
	    stmt_str.setInt( "time_base_den", ctx->streams[a]->time_base.den);
	    stmt_str.setInt( "width", ctx->streams[a]->codec->width);
	    stmt_str.setInt( "height", ctx->streams[a]->codec->height);
	    stmt_str.setInt( "gop_size", ctx->streams[a]->codec->gop_size);
	    stmt_str.setInt( "pix_fmt", ctx->streams[a]->codec->pix_fmt);
	    stmt_str.setInt( "bit_rate", ctx->streams[a]->codec->bit_rate);
	    stmt_str.setInt( "rate_emu", ctx->streams[a]->codec->rate_emu);
	    stmt_str.setInt( "sample_rate", ctx->streams[a]->codec->sample_rate);
	    stmt_str.setInt( "channels", ctx->streams[a]->codec->channels);
	    stmt_str.setInt( "sample_fmt", ctx->streams[a]->codec->sample_fmt);
//		Codec codec(ctx->streams[a]->codec->codec_id);

//	    stmt_str.setInt( "priv_data_size", codec._codec->priv_data_size);
//	    avcodec_open(ctx->streams[a]->codec, codec._codec);
//	    stmt_str.setBlob( "priv_data",(char*) ctx->streams[a]->codec->priv_data,codec._codec->priv_data_size);
//	    stmt_str.setInt( "priv_data_size", ctx->iformat->priv_data_size);
//	    stmt_str.setBlob( "priv_data",(char*) ctx->priv_data,ctx->iformat->priv_data_size);
	    stmt_str.execute();
   	    int streamid =con.lastInsertId();
   	    streams[a]=streamid;
        }
//	progress_display show_progress(duration);


    PreparedStatement stmt=con.prepareStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values "
//    "(NULL,?,?,?,?,?,?,?,?,?,?,?)");
    "(NULL,:stream_id,:pts,:dts,:stream_index,:key_frame, :frame_group,:flags,:duration,:pos,:data_size,:data)");
    while(true/*&&count < 1000*/){
		Packet packet;	
        int read=pis.readPacket(packet);
		if(count%1000==0){
			cout<<"\r" << count;
			cout.flush();
		}
        if(read<0)break;
        
        ++count;
        

	if(packet.packet->stream_index==0&&packet.isKeyFrame())frame_group++;
	int  field=0;
	packet.packet->duration=packet.packet->duration==0?1:packet.packet->duration;
        stmt.setInt( "stream_id", streams[packet.packet->stream_index]);
        stmt.setDouble( "pts", (double)packet.packet->pts);
        stmt.setDouble( "dts", (double)packet.packet->dts);
        stmt.setInt( "stream_index", packet.packet->stream_index);
        stmt.setInt( "key_frame", packet.isKeyFrame());
	if(packet.packet->stream_index==0)
    	    stmt.setInt( "frame_group", frame_group);
	else
    	    stmt.setInt( "frame_group",0);	
        stmt.setInt( "flags", packet.packet->flags);
        stmt.setInt( "duration", packet.packet->duration);
        stmt.setDouble( "pos", (double)packet.packet->pos);
        stmt.setInt( "data_size", packet.packet->size);
//        Blob blob((const char*)packet.data,packet.size);
//        stmt.setBlob( "data", (char*)packet.packet->data, packet.packet->size);
	stmt.execute();
//	show_progress+=packet.duration;
    }
    
//    trans.commit();
    cout << endl;
//	show_progress=duration;
    	return fileid;
}
/*
int main(int argc, char * argv[]){
	import(argc,argv);
}
*/
#endif

