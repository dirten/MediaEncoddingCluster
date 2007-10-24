#include "PreparedStatement.h"

using namespace org::esb::sql;

void PreparedStatement::setBlob(int pos, void*){}
void PreparedStatement::setDouble(int pos, double val){}
void PreparedStatement::setInt(int pos, int val){}
void PreparedStatement::setNull(int pos){}
void PreparedStatement::setClob(int pos, void* val){}
void PreparedStatement::setString(int pos, string val){}
bool PreparedStatement::execute(){}
ResultSet & PreparedStatement::executeQuery(){}
int PreparedStatement::executeUpdate(){}
