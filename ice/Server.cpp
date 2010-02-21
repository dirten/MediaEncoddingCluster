#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <TestUnitImpl.h>

#include <iostream>

static Ice::CommunicatorPtr communicator;
using namespace std;
static void 
onCtrlC(int)
{
    if(communicator)
    {
        try
        {
            communicator->shutdown();
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            //
            // This might occur if we receive more than one signal.
            //
        }
    }
}

int main(int argc, char* argv[]){
   int status = EXIT_SUCCESS;
  IceUtil::CtrlCHandler ctrCHandler(onCtrlC);
  communicator = Ice::initialize(argc, argv);
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            return EXIT_FAILURE;
        }
        try{
        Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("testfactory", "tcp -p 10000");
        test::testfactoryPtr tf = new testfactoryimpl();
        adapter->add(tf, communicator->stringToIdentity("testfactory"));
        adapter->activate();
        communicator->waitForShutdown();
    }
    catch(const Ice::Exception& ex)
    {
        cerr << ex << endl;
        status = EXIT_FAILURE;
    }

    if(communicator)
    {
        try
        {
            communicator->destroy();
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
            status = EXIT_FAILURE;
        }
    }

    return status;
}
