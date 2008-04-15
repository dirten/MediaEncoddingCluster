
#include <mysql/mysql.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"

using namespace org::esb::io;
using namespace org::esb::av;
using namespace std;
int main(int argc, char * argv[]){
	MYSQL *db;
	MYSQL_RES *results;
	MYSQL_ROW record;
	MYSQL_STMT * stmt;
	
	
	db=mysql_init(NULL);
	mysql_real_connect(db,"127.0.0.1","root","","hive",0,NULL,0);
//	mysql_query(db, "SELECT Host, User FROM user");
//	Statement stmt=con.createStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values (NULL,?,?,?,?,?,?,?,?,?,?,?)");
	stmt=mysql_stmt_init(db);
	string sql="insert into packets(data_size,data) values (?,?)";
	mysql_stmt_prepare(stmt,sql.c_str(), sql.length());	
	MYSQL_BIND    bind[2];



	File inputFile(argv[1]);

	FormatInputStream fis(&inputFile);
	PacketInputStream pis(&fis);
	memset(bind,0,sizeof(bind));

	Packet packet;
    while(true/*&&count < 1000*/){
        pis.readPacket(packet);
//        cout << "New Packet"<<endl;
        if(packet.packet->data==NULL)break;
//        ++count;

		bind[0].buffer_type= MYSQL_TYPE_SHORT;
		bind[0].buffer= (char *)&packet.packet->size;
		bind[0].is_null= 0;
		bind[0].length=0;	


		bind[1].buffer_type= MYSQL_TYPE_STRING;
		bind[1].buffer= (char *)packet.packet->data;
		bind[1].is_null= 0;
		bind[1].length=(long unsigned int*)&packet.packet->size;
		
		
		if (mysql_stmt_bind_param(stmt, bind))
		{
		  fprintf(stderr, " mysql_stmt_bind_param() failed\n");
		    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		      exit(0);
		      }
		if (mysql_stmt_execute(stmt))
		{
		  fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		      exit(0);
		      }
    }





	
	/*
	results = mysql_store_result(db);
	while((record = mysql_fetch_row(results))) {
	      printf("%s - %s \n", record[0], record[1]);
    }
	*/
//	mysql_free_result(results);
	if (mysql_stmt_close(stmt))
	{
	  fprintf(stderr, " failed while closing the statement\n");
	    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	      exit(0);
	      }
	mysql_close(db);
	
//return 0;
}


