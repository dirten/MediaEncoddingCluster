/* 
 * File:   TestStringOutputStream.cpp
 * Author: jhoelscher
 *
 * Created on 17. Februar 2010, 14:59
 */

#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "org/esb/io/StringOutputStream.h"
/*
 * 
 */
int main(int argc, char** argv) {
  std::string data_write;
  org::esb::io::StringOutputStream sos(data_write);
  std::string data1="test_data";
  sos.write(data1);
  sos.write(data1);
  std::cout << data_write<<std::endl;
  assert(data_write=="test_datatest_data");
  return (EXIT_SUCCESS);
}

