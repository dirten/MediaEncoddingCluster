
#include "TestFile.h"
#include "org/esb/io/File.h"
#include "org/esb/lang/Exception.h"
using namespace org::esb::io;
using namespace org::esb::lang;


CPPUNIT_TEST_SUITE_REGISTRATION(TestFile);



void TestFile::setUp(){


}


void TestFile::tearDown(){


}

void TestFile::testConstructor(){
    File * file=new File("test/test_test.cpp");
    CPPUNIT_ASSERT(file->getPath()=="test/test_test.cpp");
    delete file;
}

void TestFile::testExist(){
    File * file=new File("test/test_test.cpp");
    CPPUNIT_ASSERT(file->getPath()=="test/test_test.cpp");
    CPPUNIT_ASSERT(file->exist());
    delete file;
}

void TestFile::testIsFile(){
    File * file=new File("test/test_test.cpp");
    CPPUNIT_ASSERT(file->getPath()=="test/test_test.cpp");
    CPPUNIT_ASSERT(file->isFile());
    CPPUNIT_ASSERT(!file->isDirectory());
    delete file;
}

void TestFile::testIsDir(){
    File * file=new File("test");
    CPPUNIT_ASSERT(file->getPath()=="test");
    CPPUNIT_ASSERT(!file->isFile());
    CPPUNIT_ASSERT(file->isDirectory());
    delete file;
}


