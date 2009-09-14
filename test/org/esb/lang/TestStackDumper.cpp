#include "client/linux/handler/exception_handler.h"
namespace {

  // Callback when minidump written.
 static bool MinidumpCallback(const char *dump_path,
                              const char *minidump_id,
                              void *context,
                              bool succeeded) {
   int index = reinterpret_cast<int>(context);
   printf("%d %s: %s is dumped\n", index, __FUNCTION__, minidump_id);
//   if (index == 0) {
//     should_exit = true;
//  return true;
//   }
   // Don't process it.
   return false;
 }

int die() {
 int local_var = 1;
  *reinterpret_cast<char*>(NULL) = 1;
}

}

int main() {
  google_breakpad::ExceptionHandler exhandler(".",NULL, MinidumpCallback, NULL, true);
//  ex.WriteMinidump();
  die();
}
