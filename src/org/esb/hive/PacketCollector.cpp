
#include "PacketCollector.h"
#include "org/esb/io/File.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
#include "org/esb/config/config.h"
using namespace org::esb;
using namespace org::esb::config;
namespace org{
namespace esb{
namespace hive{

	using namespace org::esb::lang;
PacketCollector::PacketCollector(){


}

PacketCollector::~PacketCollector(){


}

void PacketCollector::run(){
    sql::Connection con(std::string(Config::getProperty("db.connection")));
	while(true){
      sql::Statement stmt=con.createStatement("select * from frame_groups fg, jobs j, files f where fg.jobid=j.id and j.outputfile=f.id and fg.sended is not null order by f.id");
      sql::ResultSet rs=stmt.executeQuery();
      while(rs.next()){
        
      
      }
      Thread::sleep2(1000);
	}
}

}}}


