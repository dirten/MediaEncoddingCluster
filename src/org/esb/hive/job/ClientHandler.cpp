#include "ClientHandler.h"
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <vector>

using namespace std;
using namespace org::esb::hive::job;
boost::mutex ClientHandler::m_mutex;

ClientHandler::ClientHandler(){
	_handler=JobHandler::getInstance();

    _con=new Connection("/tmp/hive.db");
    _stmt=new Statement(_con->createStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values (NULL,?,?,?,?,?,?,?,?,?,?,?)"));

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
ProcessUnit ClientHandler::getProcessUnit(){
	Job * job=_handler->getJob();
	if(job !=NULL){
	    return job->getNextProcessUnit();
	}
	return ProcessUnit();
}

bool ClientHandler::putProcessUnit(ProcessUnit & unit){
    {
    boost::mutex::scoped_lock scoped_lock(m_mutex);
//    cout << "Target:"<<unit._target_stream<<endl;
    sqlite3_transaction trans=_con->getTransaction();




    list< boost::shared_ptr<Packet> >::iterator it; 
//    if(unit._input_packets.size()==0)break;
//	boost::shared_ptr<Packet> p=unit._input_packets.front();
//	p->size;
    int count=0, frame_group=0;;
    for(it=unit._output_packets.begin();it!=unit._output_packets.end();it++){
        boost::shared_ptr<Packet> packet=*it;

        if(packet->data==NULL)break;
        
        ++count;
//	++show_progress;

	if(packet->stream_index==0&&packet->isKeyFrame())frame_group++;
	int  field=1;
        _stmt->bind( field++, unit._target_stream);
        _stmt->bind( field++, packet->pts);
        _stmt->bind( field++, packet->dts);
        _stmt->bind( field++, packet->stream_index);
        _stmt->bind( field++, packet->isKeyFrame());
	if(packet->stream_index==0)
    	    _stmt->bind( field++, frame_group);
	else
    	    _stmt->bind( field++);	
        _stmt->bind( field++, packet->flags);
        _stmt->bind( field++, packet->duration);
        _stmt->bind( field++, packet->pos);
        _stmt->bind( field++, packet->size);
        _stmt->bind( field++, (const void*)packet->data,packet->size);
	_stmt->execute();
    }
    cout << count << "Frames saved"<<endl;
    trans.commit();
    

    }
    return true;
    
}



