//#include <libcwd/sys.h>
//#include <libcwd/debug.h>
//#include <libcwd/type_info.h>
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"
//#include "org/esb/sql/Connection.h"

#include <iostream>
#include <signal.h>
using namespace std;
using namespace org::esb::sql;
#define builtin_return_address(addr) ((char*)__builtin_return_address(addr) + libcwd::builtin_return_address_offset)

void terminate(int val) {
  cout << "Terminate...\n" << endl;


}

class test {
public:

  test() {
    con = new Connection("mysql:db=hive2;host=localhost;user=root;passwd=root");
    pstmt1 = new PreparedStatement(con->prepareStatement("select id,filename,filename from files where id=:id"));
    pstmt2 = new PreparedStatement(con->prepareStatement("select * from files"));
    stmt1 = new Statement(con->createStatement("select * from files"));
    stmt2 = new Statement(con->createStatement("select * from files"));
    pstmt3 = new PreparedStatement(con->prepareStatement("insert into files( filename)values(:value)"));
    pstmt4 = new PreparedStatement(con->prepareStatement("insert into files( filename)values(:value)"));
    pstmt5 = new PreparedStatement(con->prepareStatement("update files set filename=:value where id=:id"));
//    pstmt6 = new PreparedStatement(con->prepareStatement("insert into files( filename)values(:value)"));
    stmt3 = new Statement(con->createStatement("insert into files( filename)values('testfilename-stmt3')"));
    stmt4 = new Statement(con->createStatement("insert into files( filename)values('testfilename-stmt3')"));
  }

  ~test() {
    delete con;
    delete pstmt1;
    delete pstmt2;
    delete stmt1;
    delete stmt2;
    delete pstmt3;
    delete pstmt4;
    delete stmt3;
    delete stmt4;
  }

  void execute1() {
    pstmt1->setInt("id",1);
    ResultSet * rs = pstmt1->executeQuery2();

    while (rs->next()) {
      rs->getString("id");
      rs->getString("filename");
      rs->getString(2);
 
//      cout << "ID:" << rs.getString("id") << "Component:" << rs.getString("filename") << "\tversion:" << rs.getString(2) << endl;
    }
  }

  void execute2() {
    ResultSet * rs = pstmt2->executeQuery2();
    while (rs->next()) {
      rs->getString("id");
      rs->getString("filename");
      rs->getString(2);
 
//      cout << "ID:" << rs.getString("id") << "Component:" << rs.getString("filename") << "\tversion:" << rs.getString(2) << endl;
    }
  }
  void execute3() {
    ResultSet * rs = stmt1->executeQuery2();
    while (rs->next()) {
      
      std::string tmp1=rs->getString("id");
      std::string tmp2=rs->getString("filename");
      std::string tmp3=rs->getString(2);
 
//      cout << "ID:" << rs.getString("id") << "Component:" << rs.getString("filename") << "\tversion:" << rs.getString(2) << endl;
    }
  }
  void execute4() {
    ResultSet *rs = stmt2->executeQuery2();
    while (rs->next()) {
      rs->getString("id");
      rs->getString("filename");
      rs->getString(2);
 
//      cout << "ID:" << rs.getString("id") << "Component:" << rs.getString("filename") << "\tversion:" << rs.getString(2) << endl;
    }
  }
  void execute5(int val) {
//    pstmt3->setString("value","testfilename-pstmt3");
    pstmt3->setInt("value",val);
    pstmt3->execute();
  }

  void execute6() {
    pstmt4->setString("value","testfilename-pstmt4");
    pstmt4->execute();
  }
  void execute7() {
    stmt3->execute();
  }
  void execute8() {
    stmt4->execute();
  }
  void execute9() {
    PreparedStatement * pst=new PreparedStatement(con->prepareStatement("select * from files where id>:id"));
    pst->setInt("id",0);
    ResultSet rs=pst->executeQuery();
    
    while (rs.next()) {
      rs.getString("id");
      rs.getString("filename");
      rs.getString(2);
 
    }
    delete pst;
  }
  void execute10() {
    Statement *pst=new Statement(con->createStatement("select * from files"));
    ResultSet rs=pst->executeQuery();
    while (rs.next()) {

      std::string tmp1=rs.getString("id");
      std::string tmp2=rs.getString("filename");
      std::string tmp3=rs.getString(2);
 
    }
    delete pst;
  }
  void execute11() {
    pstmt5->setInt("value",1);
    pstmt5->setInt("id",1);
    pstmt5->execute();
  }
private:
  Connection * con;
  PreparedStatement * pstmt1;
  PreparedStatement * pstmt2;
  Statement * stmt1;
  Statement * stmt2;

  PreparedStatement * pstmt3;
  PreparedStatement * pstmt4;
  PreparedStatement * pstmt5;
  Statement * stmt3;
  Statement * stmt4;

};

int main() {
  test t;
  for(int a=0;a<10;a++){  
    logdebug("round:"<<a);

    /*
 */
    logdebug("t.execute1()");
    t.execute1();
    logdebug("t.execute2()");
    t.execute2();
    logdebug("t.execute3()");
    t.execute3();
    logdebug("t.execute4()");
    t.execute4();
    logdebug("t.execute5()");
    t.execute5(a);
    logdebug("t.execute6()");
    t.execute6();
    logdebug("t.execute7()");
    t.execute7();
    logdebug("t.execute8()");
    t.execute8();
    logdebug("t.execute9()");
    t.execute9();
    logdebug("t.execute10()");
    t.execute10();
    logdebug("t.execute11()");
    t.execute11();
  }
  exit(0);
//  t.execute1();
  //  Debug( libcw_do.on() );               // Turn on the default Debug Object.
  //	 Debug( core_dump() );
  // Turn on all debug channels that are off.
  /*
    ForAllDebugChannels(
        if (!debugChannel.is_on())
            debugChannel.on();
        );
              
        // Print a listing of all debug channels to debug object `libcw_do'.
        Debug( list_channels_on(libcw_do) );
   */
  //       Debug( list_channels_on(libcw_do) );
  //		Debug( dc::notice.on() );             // Turn on the NOTICE Debug Channel.

  //  Debug(dc::malloc.on());
  //  Dout(dc::notice, "Hello World");
  //libcwd::location_ct(builtin_return_address(0));
  //	signal(SIGSEGV, terminate);

  //	char * tmp;
  //	delete tmp;
  char * tmp_c;
  string tmp;
Connection *con2 = new Connection("mysql:db=hive2;host=localhost;user=root;passwd=root");
Connection con("mysql:db=hive2;host=localhost;user=root;passwd=root");
  /*
    Connection con("mysql:db=hive2;host=localhost;user=root;passwd=");
    Connection *con2 = new Connection("mysql:db=hive2;host=localhost;user=root;passwd=");
    PreparedStatement * pstmt = new PreparedStatement(con2->prepareStatement("select * from files"));
    PreparedStatement * pstmt3 = new PreparedStatement(con2->prepareStatement("select * from files"));
    PreparedStatement * pstmt2 = new PreparedStatement(con2->prepareStatement("insert into files( filename)values('testfile')"));
    pstmt2->execute();
    if (true) {
      //  PreparedStatement stmt=con2->prepareStatement("select * from files");
      //  stmt.close();
      //  Statement stmt=con.createStatement("select * from config");

      ResultSet rs = pstmt->executeQuery();
      while (rs.next()) {
        cout << "ID:" << rs.getString("id") << "Component:" << rs.getString("filename") << "\tversion:" << rs.getString(2) << endl;
      }

      ResultSet rs2 = pstmt3->executeQuery();
      while (rs2.next()) {
        cout << "ID:" << rs2.getString("id") << "Component:" << rs2.getString("filename") << "\tversion:" << rs2.getString(2) << endl;
      }

    }
    delete pstmt;
    delete pstmt2;
    delete pstmt3;
   */
  /*
{
  Statement stmt=con.createStatement("select * from files");
  ResultSet rs=stmt.executeQuery();
  if(rs.next()){
    cout << "ID:"<<rs.getString("id")<<"Component:"<<rs.getString("filename")<<"\tversion:"<<rs.getString(2)<<endl;
    tmp=rs.getString("id");
    tmp_c=new char[rs.getString("id").length()+1];
    memset(tmp_c,0,rs.getString("id").length()+1);
    memcpy(tmp_c,(char*)rs.getString("id").c_str(),rs.getString("id").length());
  }
}
  cout << tmp << endl;
  delete []tmp_c;
   */
  if (false) {
    Statement * stmt = new Statement(con2->createStatement("select * from files"));
    Statement * stmt2 = new Statement(con2->createStatement("select * from config"));
    ResultSet rs = stmt->executeQuery();
    ResultSet rs2 = stmt2->executeQuery();

    if (rs.next()) {
      cout << "ID:" << rs.getString("id") << "Component:" << rs.getString("filename") << "\tversion:" << rs.getString(2) << endl;
    }



    if (rs2.next()) {
      cout << "ID:" << rs2.getString(0) << rs2.getString(1) << endl;
    }


    if (rs2.next()) {
      cout << "ID:" << rs2.getString(0) << endl;
    }

    delete stmt;
    delete stmt2;
  }

  if (false) {
    Statement stmt = con.createStatement("insert into version (component, version) values ('test','0.0.0')");
    stmt.execute();
  }

  if (false) {
    PreparedStatement pstmt = con.prepareStatement("insert into version (component, version) values ('test',:version)");
    pstmt.setString("version", string("0.1.2"));
    //  pstmt.setInt("version",1);
    pstmt.execute();
  }

  if (false) {
    //  Connection con("localhost","hive","root","");
    PreparedStatement pstmt = con.prepareStatement("select * from version where version=:version");
    pstmt.setString("version", string("0.1.1"));
    ResultSet prs = pstmt.executeQuery();
    while (prs.next()) {
      cout << "Component:" << prs.getString("component") << "\tversion:" << prs.getString("version") << endl;
    }
  }



  /*
    Statement stmt=con.createStatement("select * from version");
  // Debug( list_allocations_on(libcw_do) );
          char * t=0;
          delete t;
    ResultSet rs=stmt.executeQuery();
  
    while(rs.next()){
      cout << "Component:"<<rs.getString("component")<<"\tversion:"<<rs.getString("version")<<endl;
    }
   */
  delete con2;
  //  mysql_library_end();
  //Debug( list_allocations_on(libcw_do) );
}

