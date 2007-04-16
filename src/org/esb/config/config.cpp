static const char version[] = "$Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $";

/*
 * config.cpp $Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <config.h>

using namespace std;
using namespace org::esb::config;
/**
* Prototyping
*/
struct ltstr
{
    bool operator() (const char*s1, const char*s2)
    const{return strcmp(s1,s2)<0;}
};
static map<char*,char*,ltstr> config_map;


//void parseLine(char*line);


void Config::init(char * filename){
//  cout << "try loading configuration from "<< filename << endl;
  FILE * fp;
  char buffer[255];
//  char * buffer=new char[255];
//  memset(buffer,0,255);
  if((fp = fopen(filename,"r"))==NULL)
  {
    cout << "Configurationfile not found !!!!" <<endl;
  }
  printf("Configuration Loaded from %s\n", filename);
  while (fgets(buffer,255,fp) != NULL)
  {
    printf("Buffer:%s", buffer);
    parseLine(buffer);
  }
//  delete buffer;

//    Config::config=new Config(filename);
}
/**
 * Standard Konstruktor mit uebergabe von Dateiname
 * 
Config::Config(char*filename)
{
  Config(filename);
}
*/
/**
 * Standard Konstruktor mit uebergabe von Dateiname
 * 
 */


/**
 * retriven der Konfigurationdaten
 */
char * Config::getConfig(char * key)
{
  return config_map[key];
}
/**
* @todo überarbeiten der Config Klasse
* Fehler bei leerzeilen in der Konfiguration
*/
void Config::parseLine(char*line)
{
  if(strncmp(line,"#",1)!=0)
  {
    int seperator=0;
    int length=strcspn(line,"\n");
    if(length>0)
    {
      seperator=strcspn(line,"=");
      if(seperator>0)
      {
        char *key=new char[seperator];
        short max_copy=length-seperator-1;
        char *val=new char[max_copy];
        bzero(key,sizeof(key));
        memset(val,0,max_copy);
        strncpy(key,line,seperator);
        strncpy(val,line+(seperator+1),max_copy);
        config_map[key]=val;
	printf("Key:%s\tValue:%s\n", key, val);
      }
    }
  }
}
  
