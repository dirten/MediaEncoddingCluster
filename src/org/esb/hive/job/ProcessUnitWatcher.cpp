#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "ProcessUnitWatcher.h"
#include "ClientHandler.h"

#include "org/esb/lang/Thread.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/CodecFactory.h"


namespace org{
namespace esb{
namespace hive{
namespace job{
ProcessUnitWatcher::ProcessUnitWatcher(){
	_isStopSignal=false;
}

void ProcessUnitWatcher::onMessage(org::esb::signal::Message & msg){
	if(msg.getProperty("processunitwatcher")=="start"){
		boost::thread t(boost::bind(&ProcessUnitWatcher::start, this));
	}else
	if(msg.getProperty("processunitwatcher")=="stop"){
		_isStopSignal=true;
	}
}
void ProcessUnitWatcher::stop(){

}
void ProcessUnitWatcher::start(){	
	sql::Connection con(config::Config::getProperty("db.connection"));
	while(!_isStopSignal){
//	    logdebug("ProcessUnitWatcher cycle");
        sql::Statement stmt=con.createStatement("select id, source_stream, target_stream from process_units where send is null order by priority limit 100");
	    sql::ResultSet rs=stmt.executeQuery();
	    while(rs.next()){
			boost::shared_ptr<ProcessUnit> unit(new ProcessUnit());
			unit->id=rs.getInt("id");
			unit->_decoder=hive::CodecFactory::getStreamDecoder(rs.getInt("source_stream"));
			unit->_encoder=hive::CodecFactory::getStreamEncoder(rs.getInt("target_stream"));
			unit->_source_stream=rs.getInt("source_stream");
			unit->_target_stream=rs.getInt("target_stream");

			if(ClientHandler::addProcessUnit(unit)){
				logdebug("ProcessUnit added with id "<<rs.getInt("id"));
			}
	    }
		org::esb::lang::Thread::sleep2(1000);
	}
}



}}}}