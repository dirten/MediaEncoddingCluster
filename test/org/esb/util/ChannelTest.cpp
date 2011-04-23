/* 
 * File:   ChannelTest.cpp
 * Author: HoelscJ
 *
 * Created on 25. Februar 2011, 10:48
 */

#include <cstdlib>
#include "org/esb/util/Channel.h"
#include "org/esb/lang/Thread.h"
#include "org/esb/util/Log.h"
#include "org/esb/config/config.h"
using namespace std;
using namespace org::esb::util;


void runner(int num, Channel<int> * ch){
  LOGDEBUG("running"<<num);
  int data;
  //org::esb::lang::Thread::sleep2(1000);
  while(!ch->isClosed()){
    //LOGDEBUG("try read")
    if((*ch >> data)==true){
      //LOGDEBUG("value readed")
      //std::cout << "data:"<<data<< std::endl;
    }else{
      LOGDEBUG("runner channel closed")
    }
  }
  LOGDEBUG("exiting runner")
}

void sender(int num,Channel<int> * ch, int id){
  LOGDEBUG("starting sender id:"<<id)
  for(int a=0;a<num&&!ch->isClosed();a++){
    //LOGDEBUG("try write")
    *ch << a;
    //LOGDEBUG("value written")
  }
  LOGDEBUG("exiting sender")
}

/*
 * 
 */
int main(int argc, char** argv) {
  Log::open();
  {
  org::esb::util::Channel<int> ch;
  for(int a=0;a<100;a++){
    go(runner,a, &ch);
  }

  for(int a=0;a<100;a++){
    go(sender,1000,&ch, a);
    //org::esb::lang::Thread::sleep2(10);
  }
  org::esb::lang::Thread::sleep2(5000);
/*
  
  ch << 1;
  cout<<"channel data sended "<<endl;
  int data;

  ch.enqueue(1);
  data=ch.dequeue();
  std::cout << data<< std::endl;
  org::esb::lang::Thread::sleep2(1000);
*/
  ch.flush();
  ch.close();
  org::esb::lang::Thread::sleep2(1000);

  }
  org::esb::lang::Thread::sleep2(1000);
  LOGDEBUG("exit program")
  //org::esb::config::Config::close();
  return 0;
}

