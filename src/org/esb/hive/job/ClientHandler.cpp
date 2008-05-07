#include "org/esb/config/config.h"
#include "ClientHandler.h"
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <vector>
#include "org/esb/sql/Connection.h"
#include "org/esb/io/FileOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
//#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
//#include "tntdb/connection.h"
//#include "tntdb/connect.h"
using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::io;
using namespace org::esb::sql;
using namespace org::esb::config;


boost::mutex ClientHandler::m_mutex;
boost::mutex ClientHandler::unit_list_mutex;
map<int, boost::shared_ptr<ProcessUnit> > ClientHandler::process_unit_list;

ClientHandler::ClientHandler(){
    _handler=JobHandler::getInstance();
    Connection con(Config::getProperty("db.connection"));
    _con=new Connection(con);

	
    _stmt=new PreparedStatement(_con->prepareStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values "
    "(NULL,:stream_id,:pts,:dts,:stream_index,:key_frame, :frame_group,:flags,:duration,:pos,:data_size,:data)"));
    _stmt_fr=new PreparedStatement(_con->prepareStatement("update frame_groups set complete = now() where frame_group=:fr and stream_id=:sid"));
    _stmt_pu=new PreparedStatement(_con->prepareStatement("update process_units set send = now() where id=:id"));

}
/*
bool ClientHandler::getProcessUnit(ProcessUnit & unit){

//	boost::mutex::scoped_lock scoped_lock(m_mutex);

	Job * job=_handler->getJob();
	if(job !=NULL){
	    job->getNextProcessUnit(unit);
	    return true;
	}
	return false;
}
*/
bool ClientHandler::addProcessUnit(boost::shared_ptr<ProcessUnit>  unit){
    boost::mutex::scoped_lock scoped_lock(unit_list_mutex);
    bool result=false;
    if(process_unit_list.find(unit->id)==process_unit_list.end()){
		process_unit_list[unit->id]=unit;
		result=true;
	}
	return result;
}
void ClientHandler::fillProcessUnit(ProcessUnit * u){
    boost::mutex::scoped_lock scoped_lock(m_mutex);
/*
	logdebug("packing frame group :" <<fr_gr<<" with startts: "<<startts);
	_fis->seek(_stream_index,(int64_t)startts);
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
	logdebug("packing frame group :" <<fr_gr<<" with size:"<<size<<" !!!");
    }else{
        logdebug("no more frame groups left, setting job as completed");
        setCompleteTime(1);
    }
    */


}

ProcessUnit * ClientHandler::getProcessUnit(){
    boost::mutex::scoped_lock scoped_lock(unit_list_mutex);
	if(process_unit_list.size()>0){
		map<int, boost::shared_ptr<ProcessUnit> >::iterator it=process_unit_list.begin();
		ProcessUnit * unit=(*it).second.get();
		_stmt_pu->setInt("id",unit->id);
		_stmt_pu->execute();
//		process_unit_list.erase(unit->id);
	    return unit;
	}
	return NULL;
}

string toString(int num){
	char c[10];
	memset(&c,0,10);
	sprintf(c,"%d",num);
	return string(c);
}
bool ClientHandler::putProcessUnit(ProcessUnit & unit){
    {
    boost::mutex::scoped_lock scoped_lock(m_mutex);
    list< boost::shared_ptr<Packet> >::iterator it; 
	/*
	string filename=Config::getProperty("hive.data.dir");
	filename +="/packet_";
	filename +=toString(unit._source_stream);
	filename += "_";
	filename +=toString(unit._target_stream);
	filename += "_";
	filename +=toString(unit._frame_group);
	filename +=".data";
	
	FileOutputStream fos(filename.c_str());
	ObjectOutputStream oos(&fos);
	oos.writeObject(unit);
	fos.flush();
	*/
	_stmt_fr->setInt("fr",unit._frame_group);
	_stmt_fr->setInt("sid",unit._source_stream);
	_stmt_fr->execute();
    int count=0, frame_group=0;
    for(it=unit._output_packets.begin();it!=unit._output_packets.end();it++){
        boost::shared_ptr<Packet> packet=*it;

        if(packet->packet->data==NULL){
        	cout << "Packet data is null???"<<endl;
        }
        
        ++count;
		int  field=0;

        _stmt->setInt( "stream_id", unit._target_stream);
        _stmt->setDouble( "pts", packet->packet->pts);
        _stmt->setDouble( "dts", packet->packet->dts);
        _stmt->setInt( "stream_index", packet->packet->stream_index);
        _stmt->setInt( "key_frame", packet->isKeyFrame());
		if(packet->packet->stream_index==0)
    	    _stmt->setInt( "frame_group", frame_group);
		else
    	    _stmt->setInt( "frame_group",0);	
        _stmt->setInt( "flags", packet->packet->flags);
        _stmt->setInt( "duration", packet->packet->duration);
        _stmt->setDouble( "pos", packet->packet->pos);
        _stmt->setInt( "data_size", packet->packet->size);
        _stmt->setBlob( "data", (char *)packet->packet->data,packet->packet->size);
		_stmt->execute();
    }
    

    }
    return true;
    
}



