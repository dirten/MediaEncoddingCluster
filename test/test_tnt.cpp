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
	Connection con("mysql:/localhost/port=3306;username=root;password=root;database=hive");
	Statement stmt=con.createStatement("select * from files");
	ResultSet rs=stmt.executeQuery();
	while(rs.next()){
		cout << "test:"<<rs.getBlob("filename")<<endl;
	}
	
	PreparedStatement pstmt=con.prepareStatement("insert into packets(data_size,data) values (?,?)");
//	pstmt.setInt(0,1);
	
	File inputFile(argv[1]);

	FormatInputStream fis(&inputFile);
	PacketInputStream pis(&fis);
//	memset(bind,0,sizeof(bind));

	Packet packet;
	int count =0;
    while(true/*&&count < 1000*/){
        pis.readPacket(packet);
//        cout << "New Packet"<<endl;
        if(packet.data==NULL)break;
        ++count;
		pstmt.setInt(0,packet.size);
		pstmt.setBlob(1,packet.data,packet.size);
//		cout << packet.size <<endl;
		pstmt.execute();
	}
	
	

}

