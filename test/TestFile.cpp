
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
    CPPUNIT_ASSERT(file);
    CPPUNIT_ASSERT(file->getPath()=="test/test_test.cpp");
    delete file;
}

void TestFile::testExist(){
    File * file=new File("test/test_test.cpp");
    CPPUNIT_ASSERT(file->getPath()=="test/test_test.cpp");
    CPPUNIT_ASSERT(file->exist());
    delete file;

    File * file2=new File("/etc/shadow");
    CPPUNIT_ASSERT(file2->getPath()=="/etc/shadow");
    CPPUNIT_ASSERT(file2->exist());
    delete file2;

    File * file3=new File("blafasel");
    CPPUNIT_ASSERT(file3->getPath()=="blafasel");
    CPPUNIT_ASSERT(!file3->exist());
    CPPUNIT_ASSERT(!file3->isFile());
    CPPUNIT_ASSERT(!file3->isDirectory());
    delete file3;
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

void TestFile::testCanRead(){
    File * file=new File("/etc/shadow");
    CPPUNIT_ASSERT(!file->canRead());
    delete file;

    file=new File("/etc/passwd");
    CPPUNIT_ASSERT(file->canRead());
    delete file;
}

void TestFile::testCanWrite(){
    File * file=new File("/etc/shadow");
    CPPUNIT_ASSERT(!file->canWrite());
    delete file;

    file=new File("test/test_test.h");
    CPPUNIT_ASSERT(file->canWrite());
    delete file;
}


