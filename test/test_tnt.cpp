#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"

using namespace org::esb::sql;
int main(){
	Connection con("mysql:/localhost/port=3306;username=root;database=hive");
	Statement stmt=con.createStatement("select * from files");
	ResultSet rs=stmt.executeQuery();
	while(rs.next()){
//		cout << "test:"<<rs.getString("cgi_lohninfo")<<endl;
		cout << "test:"<<rs.getString("id")<<endl;
	}
	
//	PreparedStatement pstmt=con.prepareStatement("select * from files where id=?");
//	pstmt.setInt(0,1);
//	ResultSet prs= pstmt.executeQuery();

}

