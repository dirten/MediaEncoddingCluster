/* 
 * File:   QueueOutputStream.cpp
 * Author: HoelscJ
 *
 * Created on 16. September 2010, 14:01
 */

#include <cstdlib>
#include "org/esb/io/QueueOutputStream.h"
#include "org/esb/io/ObjectOutputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
using namespace std;
using namespace org::esb::io;
using namespace org::esb::hive::job;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  QueueOutputStream q("safmq://admin:@localhost:20200/punitout");
  ObjectOutputStream oos(&q);
  ProcessUnit unit;
  try{
  for(int a=0;a<1000000;a++)
    oos.writeObject(unit);
  }catch(std::exception ex){
    LOGERROR("Exception:"<<ex.what());
  }
  return 0;
}

