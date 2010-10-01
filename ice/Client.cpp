#include <Ice/Ice.h>
#include "Printer.h"

int main(int argc, char ** argv) {
  int status = 0;
  Ice::CommunicatorPtr ic;
  try {
    ic = Ice::initialize(argc, argv);
    Ice::ObjectPrx base=ic->stringToProxy("hello");
    Demo::PrinterPrx printer = Demo::PrinterPrx::checkedCast(base);
    if (!printer)
      throw "Invalid proxy";
    printer->printString("Hello World");

  } catch (const Ice::Exception & ex) {
    std::cerr << ex << std::endl;
    status = 1;
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    status = 1;
  }
  if (ic)
ic->destroy();
  return status;

}
