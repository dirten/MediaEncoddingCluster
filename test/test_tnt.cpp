#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"

using namespace org::esb::sql;
int main(){
	Connection con("mysql:/localhost/port=3306;username=root;password=root;database=lexdb_produktion");
	Statement stmt=con.createStatement("select * from aktiv_cgi");
	ResultSet rs=stmt.executeQuery();
	while(rs.next()){
//		cout << "test:"<<rs.getString("cgi_lohninfo")<<endl;
		cout << "test:"<<rs.getBlob("cgi_lohninfo")<<endl;
	}
}