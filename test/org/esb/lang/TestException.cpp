#include "org/esb/lang/Exception.h"
#include <iostream>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
using namespace org::esb::lang;

void signal_handler(int signo) {
  void *stack[20];
  char **functions;

  int count, i;

  // Shouldn't use printf . . . oh well
  printf("Caught signal %d\n");

  count = backtrace(stack, 20);

  functions = backtrace_symbols(stack, count);
  for (i = 0; i < count; i++) {
    printf("Frame %2d: %s\n", i, functions[i]);
  }
  free(functions);
  exit(1);
}

int die() {
  char * t;
  sprintf(t, "%s", "bla");
  int local_var = 1;
  *reinterpret_cast<char*> (NULL) = 1;
  return 0;
}

int second() {
  die();
}

int first() {
  second();
}

int main() {
  signal(SIGBUS, signal_handler);
  signal(SIGILL, signal_handler);
  signal(SIGSEGV, signal_handler);
  signal(SIGABRT, signal_handler);
  signal(SIGFPE, signal_handler);
  //first();
}
