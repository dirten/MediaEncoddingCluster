#include <iostream>
#include "org/esb/lang/Thread.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
using namespace std;
using namespace org::esb::lang;

class test1{
	public:
	void onMessage(int wait){
		Thread::sleep(wait);
		cout << "test1"<<endl;
	}

};

int main(){
	test1 test;
//	boost::bind(&test1::onMessage, test);
	boost::thread t(boost::bind(&test1::onMessage, test, 1000));
	boost::thread t2(boost::bind(&test1::onMessage, test, 2000));
	cout << "signal ready"<<endl;
	t.join();
	t2.join();
return 0;
}