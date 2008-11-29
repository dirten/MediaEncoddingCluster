
#include <org/esb/sql/my_sql.h>
#include <stdlib.h>
#include <iostream>
#include <string>
/*
#include "org/esb/io/File.h"
#include "org/esb/av/FormatInputStream.h"
#include "org/esb/av/PacketInputStream.h"
#include "org/esb/av/Packet.h"
*/
//using namespace org::esb::io;
//using namespace org::esb::av;
using namespace std;
int main(int argc, char * argv[]){
	MYSQL *db;
	MYSQL_RES *results;
	MYSQL_ROW record;
	MYSQL_STMT * stmt;
	
//	mysql_library_init(NULL,NULL,NULL);
	db=mysql_init(NULL);
        if(mysql_real_connect(db,"127.0.0.1","root","","mysql",0,NULL,0)){
          cout <<mysql_error(db)<<endl;
        }
//	mysql_query(db, "SELECT Host, User FROM user");
//	Statement stmt=con.createStatement("insert into packets(id,stream_id,pts,dts,stream_index,key_frame, frame_group,flags,duration,pos,data_size,data) values (NULL,?,?,?,?,?,?,?,?,?,?,?)");
	stmt=mysql_stmt_init(db);
//	string sql="insert into packets(data_size,data) values (?,?)";
	string sql="SELECT Host, User FROM user";
	
        
        if(mysql_stmt_prepare(stmt,sql.c_str(), sql.length())){
          cout <<mysql_stmt_error(stmt)<<endl;        
          
        }

        MYSQL_BIND bind[2];//=new MYSQL_BIND[2];
        memset(bind, 0, sizeof (bind) );


        long unsigned int length[2]={0,0};
        my_bool error[2];
        
        bind[0].buffer=new char[255];
        bind[0].buffer_length = 250;
        bind[0].length=&length[0];
        bind[0].error=&error[0];
        bind[0].buffer_type=MYSQL_TYPE_VAR_STRING;
        bind[0].is_null= 0;
        
        bind[1].buffer=new char[255];
        bind[1].buffer_length = 250;
        bind[1].length=&length[1];
        bind[1].error=&error[1];
        bind[1].buffer_type=MYSQL_TYPE_VAR_STRING;
        bind[1].is_null= 0;
        
        if(mysql_stmt_bind_result(stmt, bind)){
          cout <<mysql_stmt_error(stmt)<<endl;        
        }
//        mysql_stmt_fetch(stmt);
//	mysql_stmt_execute(stmt);
      if (mysql_stmt_execute(stmt)) {
        cout <<mysql_stmt_error(stmt)<<endl;
      }
	
        if(mysql_stmt_store_result(stmt)){
          cout <<mysql_stmt_error(stmt)<<endl;
        }
        /*
	while(mysql_stmt_fetch(stmt)!= MYSQL_NO_DATA) {
	      printf("%s - %s \n", bind[0], bind[1]);
    }
	
         */
//	mysql_free_result(results);

        if (mysql_stmt_close(stmt))
	{
	  fprintf(stderr, " failed while closing the statement\n");
	    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	      }

        delete [] (char*)bind[0].buffer;
        delete [] (char*)bind[1].buffer;
        mysql_close(db);
	mysql_library_end();
	
//return 0;
}


