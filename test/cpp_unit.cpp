#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>


using namespace std;
//using namespace CppUnit;
int main(){
    CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(suite);
    CppUnit::CompilerOutputter *outputter=new CppUnit::CompilerOutputter(&runner.result(), cerr);
//    CppUnit::CompilerOutputter outputter(&runner.result(), cerr);
    
    runner.setOutputter(outputter);
    bool wasSuccess=runner.run();
//    free(suite);
    return wasSuccess ? 0:1;

}


