/* 
 * File:   FormatTest.cpp
 * Author: HoelscJ
 *
 * Created on 31. August 2010, 13:48
 */

#include <cstdlib>
#include <string>
#include "org/esb/av/Format.h"
using namespace std;
using namespace org::esb::av;
/*
 * 
 */
int main(int argc, char** argv) {
  Format f(Format::FORMAT_VIDEO);
  Format f2(CODEC_ID_NONE);
  f.setFormatOption("","");
  return 0;
}

