#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/PreparedStatement.h"
#include "org/esb/sql/ResultSet.h"
using namespace std;
using namespace org::esb::sql;
class TestOrgEsbSql: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestOrgEsbSql);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST(testStatement);
    CPPUNIT_TEST(testStatementResult);
    CPPUNIT_TEST(testPreparedStatement);

    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConnect();
	void testStatement();
	void testStatementResult();
	void testPreparedStatement();
	
};


CPPUNIT_TEST_SUITE_REGISTRATION(TestOrgEsbSql);


void TestOrgEsbSql::setUp(){

}


void TestOrgEsbSql::tearDown(){
}

void TestOrgEsbSql::testConnect(){
    Connection con("mysql:/localhost/port=3306;username=root;password=root;database=hive");
    con.close();
}
void TestOrgEsbSql::testStatement(){
    Connection con("mysql:/localhost/port=3306;username=root;password=root;database=hive");
    Statement stmt=con.createStatement("select * from files");
    con.close();
    
}
void TestOrgEsbSql::testPreparedStatement(){
    Connection con("mysql:/localhost/port=3306;username=root;password=root;database=hive");
    PreparedStatement stmt=con.prepareStatement("select id from files");
    con.close();
    
}
void TestOrgEsbSql::testStatementResult(){
    Connection con("mysql:/localhost/port=3306;username=root;password=root;database=hive");
    Statement stmt=con.createStatement("select id, filename from files");
    ResultSet rs=stmt.executeQuery();
//    while(rs.next()){
//	rs.getInt("id");
  //  }
    con.close();

}

