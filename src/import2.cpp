
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/av/Codec.h"
#include "CreateDatabase.cpp"
#include <avformat.h>
#include <boost/progress.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp> 
#include <boost/archive/polymorphic_binary_oarchive.hpp> 



#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/sqlite3x.hpp"
using namespace std;
using namespace org::esb::io;
using namespace org::esb::sql;
using namespace org::esb::av;
using namespace boost;
using namespace sqlite3x;



int main(int argc, char * argv[]){
	cout << LIBAVCODEC_IDENT <<endl;
	if(argc!=3){
		cout << "wrong parameter count"<<endl;	
		exit(1);
	}

	File databaseFile(argv[1]);
	if(!databaseFile.exists()||!checkDatabase(databaseFile)){
		createDatabase(databaseFile);
	}else{
	    cout << "Database not found";
	}

	File inputFile(argv[2]);
	if(!inputFile.canRead()){
		cout << "Source File not found"<<endl;
	}

    

	int fileid=0,count=0, frame_group=0;

	FormatInputStream fis(&inputFile);
	PacketInputStream pis(&fis);


	Connection con(argv[1]);
	sqlite3_transaction trans=con.getTransaction();

	con.executenonquery(string("INSERT INTO files(filename) values ( '")+inputFile.getPath()+"')");
	fileid=con.insertid();

	AVFormatContext * ctx=fis.getFormatContext();
	progress_display show_progress(394000);

	int streams[ctx->nb_streams];

	Statement stmt_str=con.createStatement( "insert into streams (fileid,stream_index, stream_type,codec, codec_name,framerate,start_time,duration,time_base_num, time_base_den, width, height, gop_size, pix_fmt,bit_rate, rate_emu, sample_rate, channels, sample_fmt) values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)" );
	for(int a =0;a<ctx->nb_streams;a++){
	    int field=1;
	    stmt_str.bind( field++, fileid);
	    stmt_str.bind( field++, a);
	    stmt_str.bind( field++, ctx->streams[a]->codec->codec_type);
	    stmt_str.bind( field++, ctx->streams[a]->codec->codec_id);
	    stmt_str.bind( field++, ctx->streams[a]->codec->codec_name);
	    stmt_str.bind( field++, av_q2d(ctx->streams[a]->r_frame_rate));
	    stmt_str.bind( field++, ctx->streams[a]->start_time);
	    stmt_str.bind( field++, ctx->streams[a]->duration);
	    stmt_str.bind( field++, ctx->streams[a]->time_base.num);
	    stmt_str.bind( field++, ctx->streams[a]->time_base.den);
	    stmt_str.bind( field++, ctx->streams[a]->codec->width);
	    stmt_str.bind( field++, ctx->streams[a]->codec->height);
	    stmt_str.bind( field++, ctx->streams[a]->codec->gop_size);
	    stmt_str.bind( field++, ctx->streams[a]->codec->pix_fmt);
	    stmt_str.bind( field++, ctx->streams[a]->codec->bit_rate);
	    stmt_str.bind( field++, ctx->streams[a]->codec->rate_emu);
	    stmt_str.bind( field++, ctx->streams[a]->codec->sample_rate);
	    stmt_str.bind( field++, ctx->streams[a]->codec->channels);
	    stmt_str.bind( field++, ctx->streams[a]->codec->sample_fmt);
	    stmt_str.execute();
    	    int streamid =con.insertid();
    	    streams[a]=streamid;
        }

    Packet packet;

    Statement stmt=con.createStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values (NULL,?,?,?,?,?,?,?,?,?,?,?)");
    while(true/*&&count < 1000*/){

        pis.readPacket(packet);

        if(packet.data==NULL)break;
        
        ++count;
	++show_progress;

	if(packet.stream_index==0&&packet.isKeyFrame())frame_group++;
	int  field=1;
        stmt.bind( field++, streams[packet.stream_index]);
        stmt.bind( field++, packet.pts);
        stmt.bind( field++, packet.dts);
        stmt.bind( field++, packet.stream_index);
        stmt.bind( field++, packet.isKeyFrame());
	if(packet.stream_index==0)
    	    stmt.bind( field++, frame_group);
	else
    	    stmt.bind( field++);	
        stmt.bind( field++, packet.flags);
        stmt.bind( field++, packet.duration);
        stmt.bind( field++, packet.pos);
        stmt.bind( field++, packet.size);
        stmt.bind( field++, (const void*)packet.data,packet.size);
	stmt.execute();
    }
    trans.commit();
	
    	return 0;
}
