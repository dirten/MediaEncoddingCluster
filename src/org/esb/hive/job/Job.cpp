#include "Job.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/io/File.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"
#include <vector>
#include <iostream>
#include <list>
#include <boost/shared_ptr.hpp>
#include "org/esb/config/config.h"
#include "org/esb/util/Log.h"
using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::sql;
using namespace org::esb::av;
using namespace org::esb::io;
using namespace org::esb::config;
using namespace boost;
//int Job::_frame_group=0;
//ProcessUnit*Job::_unit=0;

//queue<ProcessUnit*> Job::_unit_queue;




Job::Job(){
	_isActive=false;

    _con=new Connection(Config::getProperty("db.connection"));
//    _con->executenonquery("PRAGMA read_uncommitted = 1");
    _stmt=new PreparedStatement(_con->prepareStatement("select data_size, data, pts, dts, duration, flags, pos, stream_index from packets where frame_group=:frame_group and stream_id=:stream_id order by dts"));
    _frStmt=new PreparedStatement(_con->prepareStatement("update frame_groups set sended=now() where frame_group=:fr and stream_id=:stream"));
//    _frame_group=1;
    _completeTime=0;


/*    
    _decoder=new Decoder(CODEC_ID_MSMPEG4V3);
//    _decoder=new Decoder(CODEC_ID_MPEG4);
    _decoder->setWidth(512);
    _decoder->setHeight(256);
    _decoder->setPixelFormat(PIX_FMT_YUV420P);
    _decoder->open();
*/
//    CodecID cid=CODEC_ID_MPEG2VIDEO;

}

Job::~Job(){
}
/*
File & Job::getSourceFile(){return *_source;}
File & Job::getTargetFile(){return *_target;}

void Job::setSourceFile(File & source){_source=&source;}
void Job::setTargetFile(File & target){_target=&target;}

*/
int Job::getStartTime(){return _startTime;}
int Job::getCompleteTime(){return _completeTime;}
void Job::setStartTime(int start){_startTime=start;}
void Job::setCompleteTime(int complete){_completeTime=complete;}
void Job::setId(int id){_id=id;}
int Job::getId(){return _id;}

void Job::setSourceStream(int id){_source_stream=id;}
void Job::setTargetStream(int id){_target_stream=id;}
int Job::getSourceStream(){return _source_stream;}
int Job::getTargetStream(){return _target_stream;}
void Job::activate(){
	logdebug("activating StreamId="<<_id);




	Connection con(Config::getProperty("db.connection"));

	{
	    PreparedStatement stmt=con.prepareStatement("select filename from files f, streams s where f.id=s.fileid and s.id=:inid");
	    stmt.setInt("inid", _source_stream);
	    ResultSet rs=stmt.executeQuery();
	    if(rs.next()){
		filename=rs.getString("filename");
		org::esb::io::File f(filename.c_str());
		if(f.exists()){
		    _fis=new FormatInputStream(&f);
		}else{
		    logerror("File not Found:"<<f.getPath());
		    return;
		}
	    }
	}

	{
		PreparedStatement stmt=con.prepareStatement("select s.codec, s.width, s.height, s.pix_fmt, s.stream_type, s.stream_index, s.channels, s.sample_rate, s.sample_fmt from job_details j, streams s where (j.instream=s.id) and j.id=:id");
		stmt.setInt("id",_id);
		ResultSet rs=stmt.executeQuery();
		if(rs.next()){
    		_decoder=new Decoder((CodecID)rs.getInt(0));
    		_decoder->setWidth(rs.getInt(1));
    		_decoder->setHeight(rs.getInt(2));
    		_decoder->setPixelFormat((PixelFormat)rs.getInt(3));
    		_decoder->setChannels(rs.getInt(6));
    		_decoder->setSampleRate(rs.getInt(7));
    		_decoder->setSampleFormat((SampleFormat)rs.getInt(8));

    		_decoder->open();
    		_stream_type=rs.getInt(4);
    		_stream_index=rs.getInt(4);
		}
	}
	{
		PreparedStatement stmt=con.prepareStatement("select s.codec, s.width, s.height, s.pix_fmt, s.bit_rate, s.time_base_num, s.time_base_den, s.gop_size, s.channels, s.sample_rate, s.sample_fmt from job_details j, streams s where (j.outstream=s.id) and j.id=:id");
		stmt.setInt("id",_id);
		ResultSet rs=stmt.executeQuery();
		if(rs.next()){
    		_encoder=new Encoder((CodecID)rs.getInt(0));
    		_encoder->setWidth(rs.getInt(1));
    		_encoder->setHeight(rs.getInt(2));
    		_encoder->setPixelFormat((PixelFormat)rs.getInt(3));
    		_encoder->setBitRate(rs.getInt(4));
			AVRational r;
			r.num=rs.getInt(5);
			r.den=rs.getInt(6);
    		_encoder->setTimeBase(r);
    		_encoder->setGopSize(rs.getInt(7));
    		_encoder->setChannels(rs.getInt(8));
    		_encoder->setSampleRate(rs.getInt(9));
    		_encoder->setSampleFormat((SampleFormat)rs.getInt(10));
    		_encoder->open();
		}
	}

	{
		string sql;
//		if(_decoder->codec_type==CODEC_TYPE_VIDEO){
//			sql="select distinct b.frame_group from (select pts from packets where stream_id=:source_stream_id except select pts from packets where stream_id=:target_stream_id) a, packets b where a.pts=b.pts and b.stream_id=:source_stream_id order by a.pts";
//		    sql="select a.frame_group  from packets a, job_details left join packets b on outstream=b.stream_id  where a.stream_id=:instream and a.stream_id=instream and b.stream_id is null group by a.frame_group;";
		    sql="select frame_group, startts, frame_count  from frame_groups where stream_id=:instream and sended is null";
//		}	
//		if(_decoder->codec_type==CODEC_TYPE_AUDIO){
//			sql="select distinct b.frame_group from (select pts from packets where stream_id=:source_stream_id except select pts from packets where stream_id=:target_stream_id) a, packets b where a.pts=b.pts and b.stream_id=:source_stream_id order by a.pts";
//		    sql="select a.frame_group  from packets a, job_details left join packets b on outstream=b.stream_id  where a.stream_id=:instream and a.stream_id=instream and b.stream_id is null group by a.frame_group;";
//		}
		PreparedStatement stmt=con.prepareStatement(sql.c_str());
		stmt.setInt("instream",_source_stream);
//		stmt.setInt("target_stream_id",_target_stream);
		ResultSet rs=stmt.executeQuery();
		while(rs.next()){
		    std::pair<int,std::pair<long long int, int> > p;
		    
		    p.first=rs.getInt(0);
		    p.second.first=rs.getLong(1);
		    p.second.second=rs.getInt(2);
		    _frame_groups.push(p);
		}
	}
	logdebug("FrameGroups:"<<_frame_groups.size());
//	cout << "FrameGroups:"<<_frame_groups.size()<<endl;
	if(_frame_groups.size()==0){
		setCompleteTime(1);
		logdebug("Job "<<getId()<<" complete");
//		cout << "Job "<<getId()<<" complete"<<endl;	
	}

}
bool Job::isActive(){
	return _isActive;
}

ProcessUnit Job::getNextProcessUnit(){
    boost::mutex::scoped_lock scoped_lock(m_mutex);
    ProcessUnit u;
    if(_frame_groups.size()>0){
	int fr_gr=_frame_groups.front().first;
	long long int startts=_frame_groups.front().second.first;
	int frame_count=_frame_groups.front().second.second;
	u._frame_group=fr_gr;
	_frame_groups.pop();
	logdebug("packing frame group :" <<fr_gr<<" with startts: "<<startts);
	_fis->seek(_stream_index,(long long int)startts);
	PacketInputStream pis(_fis);
	int size=0;
	for(int a=0;a<frame_count;){
	    Packet tmp_p;
	    pis.readPacket(tmp_p);
	    if(tmp_p.packet->stream_index!=_stream_index)continue;
	    a++;
	    shared_ptr<Packet> p(new Packet(tmp_p));
	    u._input_packets.push_back(p);
	    size+=p->packet->size;
	}
	u._decoder=_decoder;
	u._encoder=_encoder;
	u._source_stream=getSourceStream();
	u._target_stream=getTargetStream();
	logdebug("packing frame group :" <<fr_gr<<" with size:"<<size<<" !!!");
    }else{
        logdebug("no more frame groups left, setting job as completed");
        setCompleteTime(1);
    }
    return u;
}

/*
ProcessUnit Job::getNextProcessUnit(){
    {
	boost::mutex::scoped_lock scoped_lock(m_mutex);
	ProcessUnit u;
	if(_frame_groups.size()>0){
	int fr_gr=_frame_groups.front().first;
	_frame_groups.pop();
	cout << "\rFrameGroup:"<<fr_gr;
	cout.flush();
	_stmt->setInt("frame_group",fr_gr);
	_stmt->setInt("stream_id",getSourceStream());
	ResultSet rs=_stmt->executeQuery();
	while(rs.next()){
	    shared_ptr<Packet> p(new Packet(rs.getInt(0)));
	    memcpy(p->packet->data,rs.getBlob(1).c_str(),p->packet->size);
	    p->packet->pts=rs.getDouble(2);
	    p->packet->dts=rs.getDouble(3);
	    p->packet->duration=rs.getInt(4);
	    p->packet->flags=rs.getInt(5);
	    p->packet->pos=rs.getDouble(6);
	    p->packet->stream_index=rs.getInt(7);
	    u._input_packets.push_back(p);
	}
	u._decoder=_decoder;
	u._encoder=_encoder;
	u._source_stream=getSourceStream();
	u._target_stream=getTargetStream();
	u._frame_group=fr_gr;
//	_frame_group++;
	_frStmt->setInt("fr",fr_gr);
	_frStmt->setInt("stream",getSourceStream());
	_frStmt->execute();

	}else{
	    setCompleteTime(1);
	    logdebug("Job "<<getId()<<" complete");
	}
	return u;
    }
}
*/
