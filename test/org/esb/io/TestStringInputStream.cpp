/* 
 * File:   TestStringInputStream.cpp
 * Author: jhoelscher
 *
 * Created on 17. Februar 2010, 13:59
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "org/esb/io/StringInputStream.h"
#include <iostream>
/*
 * 
 */
int main(int argc, char** argv) {
  std::string data = "bla fasel";
  org::esb::io::StringInputStream sis(data);
  int length = 3;
  char *read_char_p=new char[length+1];
  memset(read_char_p,0,length+1);
  sis.read((unsigned char *)read_char_p, length);
  std::string cmp = "bla";
//  assert(cmp == read_char_p);
  delete [] read_char_p;
  std::string data2;
  sis.read(data2);
//  std::cout << data2 <<std::endl;
//  assert(data2==" fasel");
  return (EXIT_SUCCESS);
}

