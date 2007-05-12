static const char version[] = "$Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $";

/*
 * config.cpp $Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <org/esb/config/config.h>

using namespace std;
using namespace org::esb::config;
using namespace org::esb::util;
namespace org{
namespace esb{
namespace config{
/**
 * Initialisierung der Konfiguration durch eine Property Datei
 * @param filename 
 */
SimpleProperties * properties=new SimpleProperties();

string trim(string & s, string & drop = *new string(" ")){
    string r=s.erase(s.find_last_not_of(drop)+1);
    return r.erase(0,r.find_first_not_of(drop));

}

void Config::init(char * filename)
{
//  properties=new SimpleProperties();
  FILE * fp;
  char buffer[255];
  if((fp = fopen(filename,"r"))==NULL)
  {
    cout << "Configurationfile not found !!!!" <<endl;
  }
  printf("Configuration Loaded from %s\n", filename);
//    Config::properties=new SimpleProperties();
//    properties=new SimpleProperties();
  while (fgets(buffer,255,fp) != NULL)
  {
    parseLine(buffer);
  }
}

/**
 * ermitteln des Wertes zum Schlüssel
 */
char * Config::getProperty(char * key)
{
//  return (char*)org::esb::config::Config::properties->getProperty(key);
  return (char*)properties->getProperty(key);
}

void Config::parseLine(const char*line)
{
  if(strncmp(line,"#",1)!=0&&strlen(line)>0)
  {
    StringTokenizer * st=new StringTokenizer(line,"=");
    if(st->countTokens()==2){
	string key=st->nextToken();
	string val=st->nextToken();
	char *delim="\n";
//	Config::properties->setProperty(trim(key),trim(val,*new string("\n")));
	properties->setProperty(strtok((char*)key.c_str(),delim),strtok((char*)val.c_str(),delim));
//	delete [] delim;
    }
    delete st;
  }
}
}}}


