#include "org/esb/lang/StackDumper.h"
//#include "client/linux/handler/exception_handler.h"

int die() {
  char * t;
  sprintf(t,"%s", "bla");
  int local_var = 1;
  *reinterpret_cast<char*>(NULL) = 1;
  return 0;
}

using std::wstring;
int main() {
  StackDumper d=new StackDumper(".");


  char * t=new char[100000];
  char test[1000000];
  /*only for hand testing*/
  //die();
  return 0;
}
