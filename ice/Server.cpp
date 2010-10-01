#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include "Printer.h"
#include <iostream>


class PrinterI: public Demo::Printer{
public:
//  virtual void printString(const std::string & s,const Ice::Current & c);
void printString(const std::string& s, const Ice::Current& c){
  std::cout << "printString:"<<s<<std::endl;
}
void shutdown(const Ice::Current& c){
  std::cout << "shutdown"<<std::endl;
  c.adapter->getCommunicator()->shutdown();
}
};


static Ice::CommunicatorPtr communicator;
using namespace std;

static void
onCtrlC(int) {
  if (communicator) {
    try {
      communicator->shutdown();
    } catch (const Ice::CommunicatorDestroyedException&) {
      //
      // This might occur if we receive more than one signal.
      //
    }
  }
}

int main(int argc, char* argv[]) {
  int status = EXIT_SUCCESS;
  IceUtil::CtrlCHandler ctrCHandler(onCtrlC);
  communicator = Ice::initialize(argc, argv);
  if (argc > 1) {
    cerr << argv[0] << ": too many arguments" << endl;
    return EXIT_FAILURE;
  }
  try {
    Ice::ObjectAdapterPtr adapter=communicator->createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -p 10000");
    Ice::ObjectPtr printer=new PrinterI;
    adapter->add(printer, communicator->stringToIdentity("SimplePrinter"));
    adapter->activate();
    communicator->waitForShutdown();
  } catch (const Ice::Exception& ex) {
    cerr << ex << endl;
    status = EXIT_FAILURE;
  }

  if (communicator) {
    try {
      communicator->destroy();
    } catch (const Ice::Exception& ex) {
      cerr << ex << endl;
      status = EXIT_FAILURE;
    }
  }

  return status;
}
