#include "org/esb/io/File.h"
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Properties.h"
#include "Connection.h"
#include "Statement.h"
#include "PreparedStatement.h"
#include <iostream>
#include "tntdb/connect.h"
using namespace org::esb::sql;
using namespace org::esb::io;
using namespace org::esb::util;

Connection::Connection() throw (SqlException){

}
Connection::Connection(const char * con) throw (SqlException){
	tntcon=tntdb::connect(con);
}

Connection::~Connection(){
//    delete tntcon;
//    tntcon=0;
	tntcon.close();
}


Statement Connection::createStatement(const char * sql){
	return Statement(tntcon.prepare(sql));
//	_tmpStatement=Statement(*this, sql);
}

PreparedStatement Connection::prepareStatement(const char * sql){
//	PreparedStatement *stmt=new PreparedStatement(*this, sql);
	return PreparedStatement(tntcon.prepare(sql));
}

//Statement & Connection::createStatement(){return;}


void Connection::close(){
//	mysql_close(mysql);
//    sqlite3_connection::close();	
}
void Connection::executeNonQuery(const char * sql){
//    sqlite3_connection::close();	
}
long Connection::lastInsertId(){
	return tntcon.selectValue("select last_insert_id()").getInt();
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
