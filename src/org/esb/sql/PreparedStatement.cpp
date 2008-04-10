#include "PreparedStatement.h"
#include <iostream>
#include "tntdb/bits/blob.h"
using namespace org::esb::sql;

PreparedStatement::PreparedStatement(tntdb::Statement stmt):Statement(stmt){

}

PreparedStatement::~PreparedStatement(){
}

void PreparedStatement::setBlob(string pos, char* data, int length){
	tntdb::Blob lob(data, length);
	tntstmt.setBlob(pos, lob);
}

void PreparedStatement::setClob(string pos, char* data, int length){
	tntdb::Blob lob(data, length);
	tntstmt.setBlob(pos, lob);
}

void PreparedStatement::setDouble(string pos, double data){
	tntstmt.setDouble(pos, data);
}

void PreparedStatement::setInt(string pos, int data){
	tntstmt.setInt(pos, data);
}

void PreparedStatement::setNull(string pos){
	tntstmt.setNull(pos);
}

void PreparedStatement::setString(string pos, string data){
	tntstmt.setString(pos, data);
}

void PreparedStatement::setString(string pos, char * data){
	tntstmt.setString(pos, data);
}


int PreparedStatement::executeUpdate(){

}

unsigned long long PreparedStatement::getLastInsertId(){

}

