#include "org/esb/io/File.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Properties.h"
#include "Connection.h"
#include "Statement.h"
#include "PreparedStatement.h"
#include "tntdb/connect.h"
//#include <sqlite3.h>
#include <iostream>
//#include <mysql.h>
using namespace org::esb::sql;
using namespace org::esb::io;
using namespace org::esb::util;
//void __attribute__ ((constructor)) my_init(void);
//void _init() __attribute__((constructor));

Connection::Connection(char*connect_str){
	StringTokenizer toker(connect_str,":");
	if(toker.countTokens()!=2){
		cout <<  "connect_string falsch"<<endl;
		exit(0);
	}
	string type=toker.nextToken();
	if(strcmp(type.c_str(),"mysql")!=0&&strcmp(type.c_str(),"mysql_embedded")!=0){
		cout << "Database Type can only be \"mysql\" or \"mysql_embedded\""<<endl;
		exit(0);
	}
	string con_str=toker.nextToken();
	StringTokenizer info(con_str,"/");

	if(info.countTokens()<2){
		cout << "wrong connectioninfo"<<endl;
		exit(0);
	}
	string host=info.nextToken();
	string parameter=info.nextToken();
	StringTokenizer params(parameter,";");
	Properties props;
	props.setProperty("host", host);
	while(params.hasMoreTokens()){
		StringTokenizer p(params.nextToken(),"=");
		string key=p.nextToken();
		string val=p.nextToken();
		props.setProperty(key,val);
	}
	mysql=mysql_init(NULL);
	mysql_real_connect(mysql, 
		props.getProperty("host"),
		props.getProperty("username"),
		props.getProperty("password"), 
		props.getProperty("database"), 0,NULL,0);
		
	if(mysql_errno(mysql)>0){
		cout <<mysql_error(mysql)<<endl;
		exit(0);
	}
}
Connection::~Connection(){
//	mysql_close(mysql);
}

sqlite3_transaction Connection::getTransaction(){
//    return sqlite3_transaction(*this);
}

Connection::Connection(File & databaseFile)/*:sqlite3_connection(databaseFile.getPath())*/{
//    printf("Opening Database Connection\n");
}

Statement Connection::createStatement(const char * sql){
	return Statement(*this, sql);
//	_tmpStatement=Statement(*this, sql);
}
PreparedStatement & Connection::prepareStatement(const char * sql){
	PreparedStatement *stmt=new PreparedStatement(*this, sql);
	return *stmt;
}

//Statement & Connection::createStatement(){return;}


void Connection::close(){
//    sqlite3_connection::close();	
}
void Connection::executeNonQuery(const char * sql){
//    sqlite3_connection::close();	
}
long Connection::lastInsertId(){
//    sqlite3_connection::close();	
}
/*
void
//_init(int argc, char *argv[], char *envp[])
my_init()
{
    std::cout << "Lib SQL Init"<<std::endl;
}
*/
