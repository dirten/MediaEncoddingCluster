#include "org/esb/lang/StackDumper.h"
//#include "client/linux/handler/exception_handler.h"

int die() {
 int local_var = 1;
  *reinterpret_cast<char*>(NULL) = 1;
  return 0;
}


int main() {
  StackDumper stackd(".");
//  google_breakpad::ExceptionHandler exhandler(".",NULL, NULL, NULL, true);
//  exhandler.DoDump()
  die();
}
