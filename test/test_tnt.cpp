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
	Connection con("mysql:/localhost/port=3306;username=root;password=;database=hive");
	Statement stmt=con.createStatement("select filename, genre, insertdate from files limit 100");
	ResultSet rs=stmt.executeQuery();
	while(rs.next()){
		cout << "test:"<<rs.getString("filename")<<endl;
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

