#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"


#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"

#include <list>
using namespace org::esb;


int main(){
	io::File file("/mnt/Video/sortiert/Der Blutige Pfad Gottes - German (DVD-Quali).avi");
	av::FormatInputStream fis(&file);
	av::PacketInputStream pis(&fis);
	fis.dumpFormat();
	sql::Connection con("mysql:db=hive;user=root;passwd=root");
	sql::Statement stmt=con.createStatement("select min(pts) as wert from frame_groups f, packets p where f.frame_group=p.frame_group and stream_index=0 group by p.frame_group order by rand() limit 10");
	sql::ResultSet rs=stmt.executeQuery();
	std::list<int> pts;
	while(rs.next()){
		pts.push_back(rs.getInt("wert"));
	}
	
	list<int>::iterator it=pts.begin();
	av::Packet p;
	for(;it!=pts.end();it++){
		int wert=*it;
		fis.seek(0,wert);
		av::Packet p=pis.readPacket();
		av::Packet p2(500);
		cout <<"PacketPts:"<<p.packet->pts<<"\tWertPts"<< wert<<endl;
	}
}

