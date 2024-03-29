#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"

using namespace org::esb::sql;
using namespace org::esb::io;
using namespace org::esb::av;

int main(int argc, char ** argv){
//    log_init("log.properties");
    
//	Connection con("mysql:/localhost/port=3306;username=root;password=root;database=hive");
	Connection con("mysql:db=hive;user=root;passwd=");

	/*
	Statement stmt=con.createStatement("select * from packets order by id limit 10");
	ResultSet rs=stmt.executeQuery();
	while(rs.next()){
	    cout <<rs.getInt("id")<<endl;
	}
	*/

	PreparedStatement stmt=con.prepareStatement("select id from packets where stream_id=1 and frame_group=:frame_group");
//	for(int a=1;a<100;a++){
	    
	    for(int a=1;a<10;a++){
	    stmt.setInt("frame_group",a);
	    ResultSet rs=stmt.executeQuery();
	    while(rs.next()){
//		cout << "dts:"<<rs.getInt("dts");
//		cout << "\tsize:"<<rs.getInt("data_size");
//		cout << "\tdata:"<<rs.getBlob("data")<<endl;
	    }
	    cout <<"\rFrameGroup "<<a;
	    cout.flush();
	    }
//	}

	
//	PreparedStatement pstmt=con.prepareStatement("insert into packets(data_size,data) values (?,?)");
/*
	PreparedStatement pstmt=con.prepareStatement("insert into files(filename) values (?)");
	File inputFile(argv[1]);
	string tmp="bla fasel";
//	pstmt.setBlob(0,(void *)tmp.c_str(), tmp.length());
	pstmt.setString(0,tmp);
	pstmt.execute();
*/
/*
	FormatInputStream fis(&inputFile);
	PacketInputStream pis(&fis);
	Packet packet;
	int count =0;
	while(true){
    	    pis.readPacket(packet);
    	    if(packet.data==NULL)break;
    	    ++count;
	    pstmt.setInt(0,packet.size);
	    
	    pstmt.setBlob(1,(char*)packet.data,packet.size);
	    pstmt.execute();
	}*/
}

