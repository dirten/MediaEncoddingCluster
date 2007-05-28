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
    runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cerr));
    bool wasSuccess=runner.run();
    return wasSuccess ? 0:1;
//    cout << "bla"<<endl;
        

}


