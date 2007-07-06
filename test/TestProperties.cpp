#include <cppunit/extensions/HelperMacros.h>
#include "org/esb/util/SimpleProperties.cpp"
#include "org/esb/io/FileInputStream.h"
#include <iostream>

using namespace std;
using namespace org::esb::util;
using namespace org::esb::io;
class TestProperties: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestProperties);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConstructor();

};




CPPUNIT_TEST_SUITE_REGISTRATION(TestProperties);



void TestProperties::setUp(){


}


void TestProperties::tearDown(){


}
void TestProperties::testConstructor(){

	SimpleProperties p;
	
	FileInputStream is("cluster.cfg");
	p.load(&is);
	string a=p.getProperty("src.path");
	string b="/media/video";
	CPPUNIT_ASSERT_EQUAL(a,b);
}
