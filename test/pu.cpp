/* 
 * File:   pu.cpp
 * Author: jhoelscher
 *
 * Created on 29. September 2009, 13:33
 */
#include "org/esb/io/FileInputStream.h"
#include "org/esb/io/ObjectInputStream.h"
#include "org/esb/hive/job/ProcessUnit.h"
#include <stdlib.h>

/*
 * 
 */
using namespace org::esb::io;
using namespace org::esb::hive::job;

int main(int argc, char** argv) {
  char * filename = argv[1];
  FileInputStream fis(filename);
  ObjectInputStream ois(&fis);
  ProcessUnit pu;
  ois.readObject(pu);

  printf("%10s|", "inputsize");
  printf("%10s|", "outputsize");
  printf("%10s|", "decoder");
  printf("%10s|", "encoder");
  cout << endl;
  printf("%10ld|", pu._input_packets.size());
  printf("%10ld|", pu._output_packets.size());
  printf("%10ld|", pu._decoder->getCodecId());
  printf("%10ld|", pu._encoder->getCodecId());

  
  
  cout << endl;
  return (EXIT_SUCCESS);
}

