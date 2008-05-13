#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
//#include "org/esb/sql/Connection.h"

#include <iostream>
using namespace std;
using namespace org::esb::sql;
int main(){


  Connection con("mysql:db=hive;user=root;passwd=");
  Statement stmt=con.createStatement("select * from version");
  ResultSet rs=stmt.executeQuery();
  
  while(rs.next()){
    cout << "Component:"<<rs.getString("component")<<"\tversion:"<<rs.getString("version")<<endl;
  }
  mysql_library_end();
}

