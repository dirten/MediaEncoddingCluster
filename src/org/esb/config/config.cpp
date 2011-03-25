static const char version[] = "$Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $";

/*
 * config.cpp $Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "org/esb/db/hivedb.hpp"
#include "config.h"

#include "org/esb/lang/Exception.h"
#include "Defaults.cpp"
#include <stdlib.h>
#include <string.h>
using namespace std;
using namespace org::esb::config;
using namespace org::esb::util;
using namespace org::esb::lang;
//namespace org{
//namespace esb{
//namespace config{
/**
 * Initialisierung der Konfiguration durch eine Property Datei
 * @param filename 
 */
Properties * properties = new Properties();
bool Config::_isInitialized=false;
string trim(string & s, string & drop) {
  string r = s.erase(s.find_last_not_of(drop) + 1);
  return r.erase(0, r.find_first_not_of(drop));
}

void Config::close() {
  LOGDEBUG("void Config::close()");
  properties->clear();
  delete properties;
  properties = NULL;
}

bool Config::init() {
  loadDefaults(properties);
  _isInitialized=true;
  return true;
}

void Config::loadConfigFile(const std::string filename) {
  FILE * fp;
  char buffer[255];
  if ((fp = fopen(filename.c_str(), "r")) != NULL) {
    while (fgets(buffer, 255, fp) != NULL) {
      parseLine(buffer);
    }
    fclose(fp);
  }else{
//    return false;
  }
}

void Config::save2db() {
}

/**
 * ermitteln des Wertes zum Schlüssel
 */
const char * Config::getProperty(const char * key, const char * def) {
  if(!_isInitialized)init();
  if (!properties || !properties->hasProperty(key))return def;
  return (char*) properties->getProperty(key);
}

std::string Config::get(std::string key, std::string def) {
  if(!_isInitialized)init();
  if(getenv(key.c_str())!=NULL)return getenv(key.c_str());
  if (!properties || !properties->hasProperty(key))return def;
  return properties->getProperty(key);
}

void Config::setProperty(std::string key, std::string val) {
  if(!_isInitialized)init();
  properties->setProperty(key, val);
}

Properties * Config::getProperties() {
  return properties;
}

void Config::parseLine(const char*line) {
  if (strncmp(line, "#", 1) != 0 && strlen(line) > 0) {
    StringTokenizer st(line, "=");
    string key;
    string val;
    for (int a = 0; st.hasMoreTokens(); a++) {
      if (a == 0) {
        key.append(st.nextToken());
      } else {
        val.append(st.nextToken());
        if (st.hasMoreTokens())
          val.append("=");
      }
    }
    char delim[] = "\n";
    if (key.length() > 0 && val.length() > 0)
      properties->setProperty(strtok((char*) key.c_str(), delim), strtok((char*) val.c_str(), delim));

    //    delete st;
  }
}
//}}}


/*

 #include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
  char **next = envp;

  while (*next)
  {
    printf("%s\n", *next);
    next++;
  }
  return 0;


}



 */