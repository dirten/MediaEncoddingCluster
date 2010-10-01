/* 
 * File:   GridServer.cpp
 * Author: HoelscJ
 *
 * Created on 29. September 2010, 17:49
 */

#include <cstdlib>
#include <Ice/Ice.h>
#include "Printer.h"
using namespace std;

class PrinterI : public Demo::Printer {
public:

  void printString(const std::string& s, const Ice::Current& c) {
    std::cout << "printString:" << s << std::endl;
  }

  void shutdown(const Ice::Current& c) {
    std::cout << "shutdown" << std::endl;
    c.adapter->getCommunicator()->shutdown();
  }
};

class GridServer : public Ice::Application {
public:

  int run(int argc, char**argv) {
    if (argc > 1) {
      std::cerr << appName() << " to many arguments" << std::endl;
      return EXIT_FAILURE;
    }
    Ice::PropertiesPtr properties=communicator()->getProperties();
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Hello");
    Ice::Identity id = communicator()->stringToIdentity(properties->getProperty("Identity"));
    Demo::PrinterPtr hello = new PrinterI();
    adapter->add(hello, id);
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
  }

};

/*
 * 
 */
int main(int argc, char** argv) {
//
    // Set the 'BuildId' property displayed in the IceGridAdmin GUI
    //
    Ice::InitializationData initData;
    initData.properties = Ice::createProperties();
    initData.properties->setProperty("BuildId", string("Ice ") + ICE_STRING_VERSION);

    GridServer app;
    int status = app.main(argc, argv, initData);
    return status;
  return 0;
}

