#ifndef TEST_H
#define TEST_H
#include <cppunit/extensions/HelperMacros.h>

class TestFile: public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE(TestFile);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testExist);
    CPPUNIT_TEST(testIsFile);
    CPPUNIT_TEST(testIsDir);
    CPPUNIT_TEST_SUITE_END();
    
    public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testExist();
	void testIsFile();
	void testIsDir();

};



#endif

