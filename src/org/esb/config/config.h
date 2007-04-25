/*
 * aviserv.cpp
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef ESBCONFIG_H
#define ESBCONFIG_H
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Properties.h"
#include "org/esb/util/SimpleProperties.cpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <stdlib.h>
#include <map>
using namespace std;
using namespace org::esb::util;
namespace org
{
  namespace esb
  {
    namespace config
    {

      class Config
      {
      public:
        static void init(char*filename);
        static char * getProperty( char * key);
//	static SimpleProperties * properties;
      private:
        static void parseLine(const char*);
      };
    }
  }
}
#endif
