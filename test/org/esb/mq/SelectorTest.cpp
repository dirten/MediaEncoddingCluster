
#include "org/esb/mq/QueueMessage.h"
#include "org/esb/mq/Selector.h"
#include <stdio.h>
using org::esb::mq::QueueMessage;
using org::esb::mq::Selector;
using std::cout;
using std::endl;

int main(int argc, char ** argv){
  QueueMessage msg;
  msg.setLabel("test=bla info=rot");


  Selector sel("test=bla");
  if(sel.match(msg)){
    cout << "selector matches"<<endl;
  }


  return 0;
}
