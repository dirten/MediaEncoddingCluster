/*
 * TestFrame.cpp
 *
 *  Created on: 10.03.2010
 *      Author: jhoelscher
 */



#include "org/esb/av/Frame.h"

#include <assert.h>
#include <iostream>
using namespace org::esb::av;


int main(int argc, char** argv){

  Frame frame1(PIX_FMT_YUV422P, 320,240);
  frame1.setWidth(320);
  frame1.setHeight(240);
  frame1.setTimeBase(1,90000);
  frame1.setDisplayAspectRatio(16,9);
  frame1.setStorageAspectRatio(320,240);
  frame1.setPixelAspectRatio(64,45);

  std::cout <<"Frame 1:"<<&frame1<<std::endl;
  /*
  Frame frame3(PIX_FMT_YUV422P, 320,240);
  frame3.setWidth(320);
  frame3.setHeight(240);
  frame3.setTimeBase(1,90000);
  frame3.setDisplayAspectRatio(16,9);
  frame3.setStorageAspectRatio(320,240);
  frame3.setPixelAspectRatio(64,45);
  
  
  
  
  
  std::cout<<frame2.toString()<<std::endl;

  frame2=frame3;
  {
    Frame frame4(frame1);
    assert(frame1.getHeight()==frame4.getHeight());
    assert(frame1.getWidth()==frame4.getWidth());
  }

  Frame *frame5=new Frame(PIX_FMT_YUV422P, 320,240);
  Frame * frame6=new Frame(*frame5);
  delete frame5;
  delete frame6;
  std::cout <<"Frame 3:"<<&frame3<<std::endl;

  Frame frame2=frame1;
  assert(frame1.getHeight()==frame2.getHeight());
  assert(frame1.getWidth()==frame2.getWidth());
   */
  {
  Frame tmp(frame1._pixFormat,frame1.getWidth(), frame1.getHeight());
  std::cout <<"Frame 2:"<<&tmp<<std::endl;
  
  frame1=tmp;
  }
  return 0;
}
