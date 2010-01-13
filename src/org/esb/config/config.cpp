static const char version[] = "$Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $";

/*
 * config.cpp $Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "config.h"
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
  }
  /*load params from database*/
  try {
    if (std::string(getProperty("db.connection")).length() > 0) {
      Connection con(std::string(getProperty("db.connection")));
      Statement * stmt = con.createStatement();
      ResultSet * rs = stmt->executeQuery("select * from config");
      while (rs->next()) {
        if (rs->getString("config_key") != "db.connection" &&
            rs->getString("config_key") != "mode.client" &&
            rs->getString("config_key") != "mode.server" &&
            rs->getString("config_key") != "hive.base_path" &&
            rs->getString("config_key") != "hive.mode") {
          properties->setProperty(rs->getString("config_key"), rs->getString("config_val"));
          LOGDEBUG("org.esb.config.Config","ConfigKey:" << rs->getString("config_key") << " ConfigVal:" << rs->getString("config_val"));
        }
      }
      delete rs;
      delete stmt;
    }
  } catch (SqlException & ex) {
    LOGERROR("org.esb.config.Config","cant load configuration from database");
    LOGERROR("org.esb.config.Config",ex.what());
  }
  return true;
}

void Config::save2db() {
  std::vector<std::pair<std::string, std::string > > ar = properties->toArray();
  std::vector<std::pair<std::string, std::string > >::iterator it = ar.begin();
  Connection con(std::string(getProperty("db.connection")));
  PreparedStatement * stmt = con.prepareStatement2("replace into config(config_key, config_val) values (:key, :val)");
  for (; it != ar.end(); it++) {
    stmt->setString("key", it->first);
    stmt->setString("val", it->second);
    stmt->execute();
  }
  delete stmt;
}

/**
 * ermitteln des Wertes zum Schlüssel
 */
char * Config::getProperty(const char * key, char * def) {
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


