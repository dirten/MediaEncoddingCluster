
#include "org/esb/lang/Thread.h"
#include "org/esb/lang/Runnable.h"
#include <iostream>
using namespace std;

class TestThread : public Runnable{
    public:
	void run(){
	    cout<<"Running"<<endl;
	}

};

int main(int argc,char**argv){

    TestThread run;//=new TestThread();
    Thread thread(&run);    
    thread.start();
    thread.join();
}
