#include "ResultSet.h"



using namespace org::esb::sql;
ResultSet::ResultSet(Statement & stmt):sqlite3_reader(stmt.executereader()){


}
bool ResultSet::next(){


}
