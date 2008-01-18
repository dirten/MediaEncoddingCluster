#include <boost/signal.hpp>
#include <iostream>
#include "org/esb/lang/Thread.h"
using namespace std;
using namespace org::esb::lang;
class Hello {
    public:
	Hello(){
	    cout << "Hallo"<<endl;
	}
	void operator()() const 
	{ 
	    std::cout << "Hello, World!" << std::endl;
	    Thread::sleep(10000);
        } 
	static void getTest() 
	{ 
	    std::cout << "Hello, World!" << std::endl;
        } 

};
int main(){
    boost::signal<void ()> sig;
//    Hello hello;
    sig.connect(Hello()); 
    sig.connect(Hello()); 
    sig();
    cout << "signal end"<< endl;
    
    
}
