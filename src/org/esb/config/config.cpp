static const char version[] = "$Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $";

/*
 * config.cpp $Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "config.h"
#include "org/esb/db/hivedb.hpp"
#include "org/esb/sql/Connection.h"
#include "org/esb/sql/Statement.h"
#include "org/esb/sql/ResultSet.h"

#include "org/esb/lang/Exception.h"
#include "org/esb/util/Log.h"
#include "Defaults.cpp"
using namespace std;
using namespace org::esb::config;
using namespace org::esb::util;
using namespace org::esb::sql;
using namespace org::esb::lang;
//namespace org{
//namespace esb{
//namespace config{
/**
 * Initialisierung der Konfiguration durch eine Property Datei
 * @param filename 
 */
Properties * properties = new Properties();

string trim(string & s, string & drop) {
  string r = s.erase(s.find_last_not_of(drop) + 1);
  return r.erase(0, r.find_first_not_of(drop));
}

void Config::close() {
  //  logdebug("clear config");
  properties->clear();
  delete properties;
  properties = NULL;
}

bool Config::init(const std::string & filename) {
  //  properties = new Properties();
  loadDefaults(properties);
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
  /*load params from database*/
  LOGDEBUG("DbConnection:"<<getProperty("db.url"));
  try {
    if (std::string(getProperty("db.url")).length() > 0) {
      LOGDEBUG("loading config from db");
      
      db::HiveDb db("mysql",getProperty("db.url"));
      vector<db::Config> configs=litesql::select<db::Config>(db).all();
      vector<db::Config>::iterator confit=configs.begin();
      for(;confit!=configs.end();confit++){
        if ((*confit).configkey != "db.connection" &&
            (*confit).configkey != "mode.client" &&
            (*confit).configkey != "mode.server" &&
            (*confit).configkey != "hive.base_path" &&
            (*confit).configkey != "hive.mode") {
          properties->setProperty((*confit).configkey, (*confit).configval);
          LOGDEBUG("ConfigKey:" << (*confit).configkey << " ConfigVal:" << (*confit).configval);
        }
      } 
    }
  } catch (...) {
    LOGERROR("cant load configuration from database");
    return false;
  }
  return true;
}

void Config::save2db() {
  db::HiveDb db("mysql",getProperty("db.url"));

  std::vector<std::pair<std::string, std::string > > ar = properties->toArray();
  std::vector<std::pair<std::string, std::string > >::iterator it = ar.begin();
  for (; it != ar.end(); it++) {
    db::Config c(db);
    c.configkey=it->first;
    c.configval=it->second;
  }
}

/**
 * ermitteln des Wertes zum Schlüssel
 */
const char * Config::getProperty(const char * key, const char * def) {
  if (!properties || !properties->hasProperty(key))return def;
  return (char*) properties->getProperty(key);
}

std::string Config::get(std::string& key, std::string& def) {
  if (!properties || !properties->hasProperty(key))return def;
  return properties->getProperty(key);
}

void Config::setProperty(const char * key, const char * val) {
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


