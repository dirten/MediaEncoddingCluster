#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"


class RunnableTest:public org::esb::lang::Runnable{
  public:
  void run(void){
    
  }
};

int main(){
  RunnableTest r;
  org::esb::lang::Thread t(&r);
  t.start();
  t.join();
  return 0;
}



