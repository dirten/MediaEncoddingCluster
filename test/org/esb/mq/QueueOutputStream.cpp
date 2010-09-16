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
  QueueOutputStream q("safmq://admin:@localhost:9000/testqueue");
  ObjectOutputStream oos(&q);
  ProcessUnit unit;
  for(int a=0;a<1000;a++)
  oos.writeObject(unit);
  return 0;
}

