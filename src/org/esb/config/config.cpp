static const char version[] = "$Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $";

/*
 * config.cpp $Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "org/esb/db/hivedb.hpp"
#include "org/esb/config/config.h"

#include "org/esb/lang/Exception.h"
#include "org/esb/hive/DatabaseService.h"
#include <boost/foreach.hpp>

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
Properties * Config::properties = NULL;
bool Config::_isInitialized=false;
std::map<std::string, char *> Config::_free_map;
string trim(string & s, string & drop) {
  string r = s.erase(s.find_last_not_of(drop) + 1);
  return r.erase(0, r.find_first_not_of(drop));
}

void Config::close() {
  //LOGDEBUG("void Config::close()");
  typedef std::vector< std::pair< std::string, std::string > > array;
  typedef std::pair< std::string, char* > row;
    if(properties){
   array props=properties->toArray();
  //std::vector< std::pair< std::string, std::string > >::iterator it=props.begin();
  
  BOOST_FOREACH(row p, _free_map) {
    //if(getenv(p.first.c_str())){
      //LOGDEBUG("delete env key"<<p.first.c_str());
      //LOGDEBUG("delete env val"<<getenv(p.first.c_str()));
      /*diese methode ist auf windows nicht verfügbar*/
      //unsetenv(p.first.c_str());
      delete [] p.second;
    //}
  }

  properties->clear();
  delete properties;
    }
  properties = NULL;
}

bool Config::init() {
  if(!_isInitialized){
  properties = new Properties();
  //loadDefaults(properties);
  }
  _isInitialized=true;
  //loadFromDb();
  return true;
}
/*
void Config::loadFromDb() {
  if(org::esb::hive::DatabaseService::databaseExist()){
    db::HiveDb db=org::esb::hive::DatabaseService::getDatabase();
    vector<db::Config> cfg=litesql::select<db::Config>(db).all();
    vector<db::Config>::iterator it=cfg.begin();
    for(;it!=cfg.end();it++){
      properties->setProperty((*it).configkey,(*it).configval);
    }
  }
}
*/
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
  std::string env=std::string(key).append("=").append(val);
  char * pa=new char[env.length()+1];
  memset(pa,0,env.length()+1);
  memcpy(pa,env.c_str(),env.length());
  putenv(pa);
  _free_map[key]=pa;

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
