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
//namespace org{
//namespace esb{
//namespace config{
/**
 * Initialisierung der Konfiguration durch eine Property Datei
 * @param filename 
 */
Properties * properties=0;

string trim(string & s, string & drop ){
    string r=s.erase(s.find_last_not_of(drop)+1);
    return r.erase(0,r.find_first_not_of(drop));

}
void Config::close(){
    delete properties;
}

void Config::init(char * filename)
{
  FILE * fp;
  char buffer[255];
  if((fp = fopen(filename,"r"))==NULL)
  {
    cout << "Configurationfile not found !!!!" <<endl;
    exit(1);
  }
//  printf("Configuration Loaded from %s\n", filename);
  properties=new Properties();
  while (fgets(buffer,255,fp) != NULL)
  {
    parseLine(buffer);
  }
  fclose(fp);
}

/**
 * ermitteln des Wertes zum Schlüssel
 */
char * Config::getProperty(char * key)
{
  if(!properties->hasProperty(key))return NULL;
  return (char*)properties->getProperty(key);
}

Properties * Config::getProperties()
{
  return properties;
}

void Config::parseLine(const char*line)
{
  if(strncmp(line,"#",1)!=0&&strlen(line)>0)
  {
    StringTokenizer st(line,"=");
    string key;
    string val;
    for(int a=0;st.hasMoreTokens();a++){
	if(a==0){
	    key.append(st.nextToken());
	}else{
	    val.append(st.nextToken());
	    if(st.hasMoreTokens())
		val.append("=");
	}
    }
    char delim[]="\n";
    properties->setProperty(strtok((char*)key.c_str(),delim),strtok((char*)val.c_str(),delim));
    
//    delete st;
  }
}
//}}}


