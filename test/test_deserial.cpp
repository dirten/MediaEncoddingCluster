#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"



using namespace org::esb::io;
using namespace org::esb::hive::job;


int main(int argc, char ** argv){

     ProcessUnit unit2;   
     FileInputStream ffis("new.unit");
     ObjectInputStream ois(&ffis);
     ois.readObject(unit2);

   //  unit2.process();
  delete unit2._decoder;


}