/*
 * aviserv.cpp
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef ESBCONFIG_H
#define ESBCONFIG_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <stdlib.h>
#include <map>
using namespace std;
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
        static char * getConfig( char * key);
      private:
        static void parseLine(char*);
      };
    }
  }
}
#endif
