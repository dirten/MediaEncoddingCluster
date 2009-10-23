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
        static bool init(const std::string & filename);
        static void close();
        static void save2db();
        static char * getProperty( const char * key, char * def="");
        static std::string get(std::string&key, std::string & def);
        static void setProperty(const char * key, const char * val);

        static Properties * getProperties();
        static void reload();
      private:
        static void parseLine(const char*);
      };
    }
  }
}
#endif
