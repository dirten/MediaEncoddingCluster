#include "ResultSet.h"



using namespace org::esb::sql;

ResultSet::ResultSet(Statement & stmt):sqlite3_reader(stmt.executereader()){}

ResultSet::ResultSet(const ResultSet & rs):sqlite3_reader(rs){}

bool ResultSet::next(){return read();}
