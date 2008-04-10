#include "org/esb/io/File.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Properties.h"
#include "Connection.h"
#include "Statement.h"
#include "PreparedStatement.h"
#include <iostream>

using namespace org::esb::sql;
using namespace org::esb::io;
using namespace org::esb::util;

Connection::Connection(const char*connect_str) throw (SqlException){

	logdebug("Opennig Connection with "<<connect_str);
	StringTokenizer toker(connect_str,":");
	if(toker.countTokens()!=2){
		throw SqlException("connect String is wrong");
	}
	string type=toker.nextToken();
	if(strcmp(type.c_str(),"mysql")!=0&&strcmp(type.c_str(),"mysql_embedded")!=0){
		throw SqlException("Database Type can only be \"mysql\" or \"mysql_embedded\"");
	}
	string con_str=toker.nextToken();
	StringTokenizer info(con_str,"/");

	if(info.countTokens()<2){
		throw SqlException("wrong connectioninfo");
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
	mysql=boost::shared_ptr<MYSQL>(mysql_init(NULL));
	mysql_real_connect(mysql.get(), 
		props.getProperty("host"),
		props.getProperty("username"),
		props.getProperty("password"), 
		props.getProperty("database"), 0,NULL,0);
		
	if(mysql_errno(mysql.get())>0){
		throw SqlException(string("Connection failed !!! ").append(mysql_error(mysql.get())));
	}
}

Connection::~Connection(){
    

}

//sqlite3_transaction Connection::getTransaction(){
//    return sqlite3_transaction(*this);
//}

//Connection::Connection(File & databaseFile)/*:sqlite3_connection(databaseFile.getPath())*/{
//    printf("Opening Database Connection\n");
//}

Statement Connection::createStatement(const char * sql){
	return Statement(*this, sql);
//	_tmpStatement=Statement(*this, sql);
}

PreparedStatement Connection::prepareStatement(const char * sql){
//	PreparedStatement *stmt=new PreparedStatement(*this, sql);
	return PreparedStatement(*this, sql);
}

//Statement & Connection::createStatement(){return;}


void Connection::close(){
//	mysql_close(mysql.get());
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
