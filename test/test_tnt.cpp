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
	Connection con("mysql:db=hive;user=root;passwd=root");

	PreparedStatement stmt=con.prepareStatement("select * from profiles limit 100");
	ResultSet rs=stmt.executeQuery();
//	log_trace("bla fasel");
	while(rs.next()){
		cout << "test:"<<rs.getString(0)<<endl;
		cout << "test:"<<rs.getString(1)<<endl;
		cout << "test:"<<rs.getString(2)<<endl;
		cout << "test:"<<rs.getString(3)<<endl;
		cout << "test:"<<rs.getString(4)<<endl;
		cout << "test:"<<rs.getString(5)<<endl;
		cout << "test:"<<rs.getString(6)<<endl;
		cout << "test:"<<rs.getString(7)<<endl;
		cout << "test:"<<rs.getString(8)<<endl;
		cout << "test:"<<rs.getString(9)<<endl;
		cout << "test:"<<rs.getString(10)<<endl;
		cout << "test:"<<rs.getString(11)<<endl;
	}
	
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

