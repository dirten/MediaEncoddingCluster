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
using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::sql;
using namespace org::esb::av;
using namespace org::esb::io;
using namespace boost;
//int Job::_frame_group=0;
//ProcessUnit*Job::_unit=0;

//queue<ProcessUnit*> Job::_unit_queue;




Job::Job(){
    _con=new Connection("/tmp/hive.db");
    _con->executenonquery("PRAGMA read_uncommitted = 1");
    _stmt=new Statement(_con->createStatement("select data_size, data, pts, dts, duration, flags, pos, stream_index from packets where frame_group=? and stream_id=?"));
    _frame_group=1;
    _completeTime=NULL;
    
    _decoder=new Decoder(CODEC_ID_MSMPEG4V3);
    _decoder->setWidth(512);
    _decoder->setHeight(256);
    _decoder->setPixelFormat(PIX_FMT_YUV420P);
    _decoder->open();

    CodecID cid=CODEC_ID_MPEG2VIDEO;
//    CodecID cid=CODEC_ID_MSMPEG4V3;
    _encoder=new Encoder(cid);
    _encoder->setBitRate(400000);
    _encoder->setTimeBase((AVRational){1,25});
//    _encoder->gop_size=10;
    _encoder->setGopSize(50);
    _encoder->setPixelFormat(PIX_FMT_YUV420P);
//    encoder->mb_decision=20;
    _encoder->setWidth(512);
    _encoder->setHeight(256);
    _encoder->open();

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
ProcessUnit Job::getNextProcessUnit(){
    {
	boost::mutex::scoped_lock scoped_lock(m_mutex);
	_stmt->bind(1,_frame_group);
	_stmt->bind(2,getSourceStream());
	ResultSet rs=_stmt->executeQuery();
	ProcessUnit u;
	if(!rs.next()){
	    setCompleteTime(1);
	    cout << "Job "<<getId()<<" complete"<<endl;
	    return u;
	}
	do{
	    shared_ptr<Packet> p(new Packet());
	    p->size=rs.getint(0);
	    p->data=new uint8_t[p->size];
	    memcpy(p->data,rs.getblob(1).c_str(),p->size);
	    p->pts=rs.getint(2);
	    p->dts=rs.getint(3);
	    p->duration=rs.getint(4);
	    p->flags=rs.getint(5);
	    p->pos=rs.getint(6);
	    p->stream_index=rs.getint(7);
	    u._input_packets.push_back(p);
	}while(rs.next());
	u._decoder=_decoder;
	u._encoder=_encoder;
	u._source_stream=getSourceStream();
	u._target_stream=getTargetStream();
	_frame_group++;
	return u;
    }
}

