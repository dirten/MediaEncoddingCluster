#include "client/linux/handler/exception_handler.h"

int die() {
 int local_var = 1;
  *reinterpret_cast<char*>(NULL) = 1;
}

int main() {
  google_breakpad::ExceptionHandler exhandler(".",NULL, MinidumpCallback, NULL, true);
  die();
}
