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
#include "org/esb/lang/Thread.h"
using namespace std;
using namespace org::esb::io;
using namespace org::esb::hive::job;

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open("");
  do {
    org::esb::av::FormatBaseStream::initialize();
    QueueInputStream qis("safmq://admin:@localhost:20200/punitout");
    ObjectInputStream ois(&qis);
    while (true) {
      ProcessUnit unit;
      if (ois.readObject(unit) != 0)
        break;
      //unit.process();
    }
    org::esb::lang::Thread::sleep2(2000);
  } while (false);
    return 0;
}

