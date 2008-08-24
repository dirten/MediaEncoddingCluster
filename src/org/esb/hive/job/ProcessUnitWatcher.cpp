#include "ProcessUnitWatcher.h"
#include "ClientHandler.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
#include "org/esb/hive/CodecFactory.h"


using namespace org::esb;
namespace org{
namespace esb{
namespace hive{
namespace job{
ProcessUnitWatcher::ProcessUnitWatcher(){
	_isStopSignal=false;
}

void ProcessUnitWatcher::onMessage(org::esb::signal::Message & msg){


}
void ProcessUnitWatcher::run(){
	
	sql::Connection con(config::Config::getProperty("db.connection"));
    sql::Statement stmt=con.createStatement("select * from process_units where send is null order by priority limit 100");
	while(!_isStopSignal){
	    logdebug("ProcessUnitWatcher cycle");
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
    	Thread::sleep(2000);
	}

}



}}}}