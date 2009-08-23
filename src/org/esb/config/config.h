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
//#include "org/esb/util/Properties.h"
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
        static bool init(char*filename);
        static void close();
        static void save2db();
        static char * getProperty( char * key, char * def="");
        static void setProperty(const char * key, const char * val);
        static Properties * getProperties();
      private:
        static void parseLine(const char*);
      };
    }
  }
}
#endif
