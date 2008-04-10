#include "Job.h"
#include "org/esb/av/Frame.h"
#include "org/esb/av/FrameInputStream.h"
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
    _stmt=new PreparedStatement(_con->prepareStatement("select data_size, data, pts, dts, duration, flags, pos, stream_index from packets where frame_group=? and stream_id=?"));
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



	Connection con(Config::getProperty("db.connection"));

	{
		PreparedStatement stmt=con.prepareStatement("select s.codec, s.width, s.height, s.pix_fmt from job_details j, streams s where (j.instream=s.id) and j.id=?");
		stmt.setInt(1,_id);
		ResultSet rs=stmt.executeQuery();
		if(rs.next()){
    		_decoder=new Decoder((CodecID)rs.getInt(0));
    		_decoder->setWidth(rs.getInt(1));
    		_decoder->setHeight(rs.getInt(2));
    		_decoder->setPixelFormat((PixelFormat)rs.getInt(3));
    		_decoder->open();
		}
	}
	{
		PreparedStatement stmt=con.prepareStatement("select s.codec, s.width, s.height, s.pix_fmt, s.bit_rate, s.time_base_num, s.time_base_den, s.gop_size, s.channels, s.sample_rate, s.sample_fmt from job_details j, streams s where (j.outstream=s.id) and j.id=?");
		stmt.setInt(1,_id);
		ResultSet rs=stmt.executeQuery();
		if(rs.next()){
    		_encoder=new Encoder((CodecID)rs.getInt(0));
    		_encoder->setWidth(rs.getInt(1));
    		_encoder->setHeight(rs.getInt(2));
    		_encoder->setPixelFormat((PixelFormat)rs.getInt(3));
    		_encoder->setBitRate(rs.getInt(4));
    		_encoder->setTimeBase((AVRational){rs.getInt(5),rs.getInt(6)});
    		_encoder->setGopSize(rs.getInt(7));
    		_encoder->setChannels(rs.getInt(8));
    		_encoder->setSampleRate(rs.getInt(9));
    		_encoder->setSampleFormat((SampleFormat)rs.getInt(10));
    		_encoder->open();
		}
	}

	{
		string sql;
		if(_decoder->codec_type==CODEC_TYPE_VIDEO){
			sql="select distinct b.frame_group from (select pts from packets where stream_id=? except select pts from packets where stream_id=?) a, packets b where a.pts=b.pts and b.stream_id=? order by a.pts";
		}	
		if(_decoder->codec_type==CODEC_TYPE_AUDIO){
			sql="select distinct b.frame_group from (select pts from packets where stream_id=? except select pts from packets where stream_id=?) a, packets b where a.pts=b.pts and b.stream_id=? order by a.pts";
		}
		PreparedStatement stmt=con.prepareStatement(sql.c_str());
		stmt.setInt(1,_source_stream);
		stmt.setInt(2,_target_stream);
		stmt.setInt(3,_source_stream);
		ResultSet rs=stmt.executeQuery();
		while(rs.next()){
			_frame_groups.push(rs.getInt(0));
		}
	}
	
	cout << "FrameGroups:"<<_frame_groups.size()<<endl;
	if(_frame_groups.size()==0){
		setCompleteTime(1);
		cout << "Job "<<getId()<<" complete"<<endl;	
	}

}
bool Job::isActive(){
	return _isActive;
}
/*
void Job::addJobDetails(JobDetail & detail){
        list<JobDetail*>::iterator i;
        _detailList.push_back(&detail);
}

bool Job::getNextProcessUnit(ProcessUnit & unit){
    {
	boost::mutex::scoped_lock scoped_lock(m_mutex);
	bool result=false;
	if(_unit_queue.size()>0){
	    unit = *_unit_queue.front();
	    _unit_queue.pop();
	    result=true;
	}
	return result;
    }
}
*/
//boost::mutex Job::m_mutex;

ProcessUnit Job::getNextProcessUnit(){
    {
	boost::mutex::scoped_lock scoped_lock(m_mutex);
	ProcessUnit u;
	if(_frame_groups.size()>0){
	int fr_gr=_frame_groups.front();
	_frame_groups.pop();
	cout << "\rFrameGroup:"<<fr_gr;
	cout.flush();
	_stmt->setInt(1,fr_gr);
	_stmt->setInt(2,getSourceStream());
	ResultSet rs=_stmt->executeQuery();
	while(rs.next()){
	    shared_ptr<Packet> p(new Packet());
	    p->size=rs.getInt(0);
	    p->data=new uint8_t[p->size];
	    memcpy(p->data,rs.getBlob(1).c_str(),p->size);
	    p->pts=rs.getInt(2);
	    p->dts=rs.getInt(3);
	    p->duration=rs.getInt(4);
	    p->flags=rs.getInt(5);
	    p->pos=rs.getInt(6);
	    p->stream_index=rs.getInt(7);
	    u._input_packets.push_back(p);
	}
	u._decoder=_decoder;
	u._encoder=_encoder;
	u._source_stream=getSourceStream();
	u._target_stream=getTargetStream();
//	_frame_group++;
	}else{
	    setCompleteTime(1);
	    cout << "Job "<<getId()<<" complete"<<endl;	
	}
	return u;
    }
}

