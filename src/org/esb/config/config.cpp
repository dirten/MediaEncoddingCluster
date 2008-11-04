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
Properties * properties = new Properties();;

string trim(string & s, string & drop) {
  string r = s.erase(s.find_last_not_of(drop) + 1);
  return r.erase(0, r.find_first_not_of(drop));

}

void Config::close() {
  delete properties;
}

void Config::init(char * filename) {
  FILE * fp;
  char buffer[255];
  if ((fp = fopen(filename, "r")) == NULL) {
    throw Exception(__FILE__, __LINE__, string("Configurationfile \"").append(filename).append("\" not found !!!!").c_str());
  }
//  properties = new Properties();
  while (fgets(buffer, 255, fp) != NULL) {
    parseLine(buffer);
  }
  fclose(fp);
  /*load params from database*/

  Connection con(getProperty("db.connection"));
  Statement stmt = con.createStatement("select * from config");
  ResultSet rs = stmt.executeQuery();
  while (rs.next()) {
    if (rs.getString("config_key") != "db.connection")
      properties->setProperty(rs.getString("config_key"), rs.getString("config_val"));
  }
}

void Config::save2db() {
  std::vector<std::pair<std::string, std::string > > ar = properties->toArray();
  std::vector<std::pair<std::string, std::string > >::iterator it = ar.begin();
  Connection con(getProperty("db.connection"));
  PreparedStatement stmt = con.prepareStatement("replace into config(config_key, config_val) values (:key, :val)");
  for (; it != ar.end(); it++) {
    stmt.setString("key", it->first);
    stmt.setString("val", it->second);
    stmt.execute();
  }
}

/**
 * ermitteln des Wertes zum Schlüssel
 */
char * Config::getProperty(char * key, char * def) {
  if (!properties->hasProperty(key))return def;
  return (char*) properties->getProperty(key);
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
      }else {
        val.append(st.nextToken());
        if (st.hasMoreTokens())
          val.append("=");
      }
    }
    char delim[] = "\n";
    properties->setProperty(strtok((char*) key.c_str(), delim), strtok((char*) val.c_str(), delim));

    //    delete st;
  }
}
//}}}


