#include "org/esb/config/config.h"
#include "ClientHandler.h"
#include "ProcessUnit.h"
#include "JobHandler.h"
#include <vector>
#include "org/esb/sql/Connection.h"
//#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
//#include "tntdb/connection.h"
//#include "tntdb/connect.h"
using namespace std;
using namespace org::esb::hive::job;
using namespace org::esb::sql;
using namespace org::esb::config;
boost::mutex ClientHandler::m_mutex;

ClientHandler::ClientHandler(){
    _handler=JobHandler::getInstance();
    Connection con(Config::getProperty("db.connection"));
    _con=new Connection(con);
    _stmt=new PreparedStatement(_con->prepareStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values (NULL,?,?,?,?,?,?,?,?,?,?,?)"));

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
//    sqlite3_transaction trans=_con->getTransaction();




    list< boost::shared_ptr<Packet> >::iterator it; 
//    if(unit._input_packets.size()==0)break;
//	boost::shared_ptr<Packet> p=unit._input_packets.front();
//	p->size;
    int count=0, frame_group=0;
    for(it=unit._output_packets.begin();it!=unit._output_packets.end();it++){
        boost::shared_ptr<Packet> packet=*it;

        if(packet->data==NULL){
        	cout << "Packet data is null???"<<endl;
        }
        
        ++count;
	int  field=0;
        _stmt->setInt( field++, unit._target_stream);
        _stmt->setInt( field++, packet->pts);
        _stmt->setInt( field++, packet->dts);
        _stmt->setInt( field++, packet->stream_index);
        _stmt->setInt( field++, packet->isKeyFrame());
	if(packet->stream_index==0)
    	    _stmt->setInt( field++, frame_group);
	else
    	    _stmt->setInt( field++,0);	
        _stmt->setInt( field++, packet->flags);
        _stmt->setInt( field++, packet->duration);
        _stmt->setInt( field++, packet->pos);
        _stmt->setInt( field++, packet->size);
        _stmt->setBlob( field++, (char *)packet->data,packet->size);
	_stmt->execute();
    }
    

    }
    return true;
    
}



