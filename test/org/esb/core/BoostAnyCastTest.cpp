/* 
 * File:   BoostAnyCastTest.cpp
 * Author: HoelscJ
 *
 * Created on 5. Oktober 2011, 15:54
 */

#include <cstdlib>
#include <boost/any.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <iostream>
using namespace std;

void mytest(std::string t){
  std::cout << t << std::endl;
}

typedef boost::function<void(std::string)> myHandler;
void mywait(myHandler handler)
{
 handler("hello my wait");
}

/*
 * 
 */
int main(int argc, char** argv) {
  //mywait((boost::bind(mytest,_1)("data")));
  return 0;
}

