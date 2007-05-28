
#include "test_test.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Exception.h"
using namespace org::esb::io;
using namespace org::esb::lang;


CPPUNIT_TEST_SUITE_REGISTRATION(TestTest);



void TestTest::setUp(){


}


void TestTest::tearDown(){


}

void TestTest::testConstructor(){
    File * file=new File("test/test_test.cpp");
    CPPUNIT_ASSERT(file->getPath()=="test/test_test.cpp");
    delete file;
}

void TestTest::testExist(){
    File * file=new File("test/test_test.cpp");
    CPPUNIT_ASSERT(file->getPath()=="test/test_test.cpp");
    CPPUNIT_ASSERT(file->exist());
    delete file;
}

void TestTest::testIsFile(){
    File * file=new File("test/test_test.cpp");
    CPPUNIT_ASSERT(file->getPath()=="test/test_test.cpp");
    CPPUNIT_ASSERT(file->isFile());
    CPPUNIT_ASSERT(!file->isDirectory());
    delete file;
}

void TestTest::testIsDir(){
    File * file=new File("test");
    CPPUNIT_ASSERT(file->getPath()=="test");
    CPPUNIT_ASSERT(!file->isFile());
    CPPUNIT_ASSERT(file->isDirectory());
    delete file;
}


