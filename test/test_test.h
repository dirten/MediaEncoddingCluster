#ifndef TEST_H
#define TEST_H
#include <cppunit/extensions/HelperMacros.h>

class TestTest: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestTest);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	
	void testConstructor();

};



#endif

