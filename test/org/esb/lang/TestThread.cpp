#include "org/esb/lang/Runnable.h"
#include "org/esb/lang/Thread.h"

bool started=false;
class RunnableTest:public org::esb::lang::Runnable{
  public:
  void run(void){
    started=true;
  }
};

int main(){
  RunnableTest r;
  org::esb::lang::Thread t(&r);
  t.start();
  t.join();
  assert(started);
  return 0;
}



