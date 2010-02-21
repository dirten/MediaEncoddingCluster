#include "TestUnit.h"

class testclassimpl:public test::testclass{
public:
   virtual void operate();
};
class testfactoryimpl:public test::testfactory{
public:
  virtual testclassimpl * getTestClass();
};
