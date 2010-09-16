/* 
 * File:   QueueInputStream.cpp
 * Author: HoelscJ
 *
 * Created on 16. September 2010, 15:33
 */

#include <cstdlib>
#include "org/esb/io/QueueInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include "org/esb/util/Log.h"
#include "org/esb/av/FormatBaseStream.h"
using namespace std;
using namespace org::esb::io;
using namespace org::esb::hive::job;
/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  org::esb::av::FormatBaseStream::initialize();
  QueueInputStream qis("safmq://admin:@localhost:9000/testqueue");
  ObjectInputStream ois(&qis);
  while(true){
    ProcessUnit unit;
    ois.readObject(unit);
    unit.process();
  }
  return 0;
}

