/*
 * aviserv.cpp
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef ORG_ESB_CONFIG_H
#define ORG_ESB_CONFIG_H
#include "org/esb/util/StringTokenizer.h"
#include "org/esb/util/Properties.h"
#include "org/esb/util/Log.h"
//#include "org/esb/util/Properties.h"
#include <iostream>
#include <stdlib.h>
#include <map>
#include "../core/exports.h"
using namespace std;
using namespace org::esb::util;
namespace org
{
  namespace esb
  {
    namespace config
    {

      class CORE_EXPORT Config
      {
        classlogger("org.esb.config.Config")
      public:
        static void close();
        static void save2db();
        static const char * getProperty( const char * key, const char * def="");
        static std::string get(std::string key, std::string def=std::string());
        static void setProperty(std::string key, std::string val);

        static Properties * getProperties();
        static void reload();
        static void loadConfigFile(const std::string filename);
        //static void loadFromDb();
        static bool init();
      private:
        static void parseLine(const char*);
        static bool _isInitialized;
        static Properties * properties;
        static std::map<std::string, char*> _free_map;
      };
    }
  }
}
#endif
