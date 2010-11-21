/* 
 * File:   TestEuclid.cpp
 * Author: jholscher
 *
 * Created on 31. Oktober 2009, 22:02
 */

#include <stdlib.h>
#include <iostream>
/*
 * 
 */
int euclid(int a, int b) {
  std::cout << "cycle(" << a << "," << b <<")"<< std::endl;
  if (b == 0)
    return a;
  else
    return euclid(b, a % b);
}

int main(int argc, char** argv) {
  std::cout <<"a=48000,b=44100:"<<euclid(120,100)<<std::endl;
  return (EXIT_SUCCESS);
}

