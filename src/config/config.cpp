static const char version[] = "$Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $";

/*
 * config.cpp $Id: config.cpp,v 1.3 2006/03/14 15:41:23 framebuster Exp $
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <iostream>
#include <stdlib.h>
#include <config.h>
#include <map>

using namespace std;

struct ltstr
{
  bool operator()
  (const char*s1, const char*s2)
  const{return strcmp(s1,s2)<0;}
};

map<char*,char*,ltstr> config_map;
/**
* Prototyping
*/
void parseLine(char*line);

/**
 * Standard Konstruktor mit uebergabe von Dateiname
 * 
*/
EsbConfig::EsbConfig(char*filename)
{
  EsbConfig(filename, true);
}
/**
 * Standard Konstruktor mit uebergabe von Dateiname
 * 
 */

EsbConfig::EsbConfig(char*filename, bool exiting)
{
  cout << "try loading configuration from "<< filename << endl;
  FILE * fp;
  char buffer[255];
 // char * buffer=new char[255];
  if((fp = fopen(filename,"r"))==NULL)
  {
    cout << "Configurationfile not found !!!!" <<endl;
    if(exiting)
    {
      exit(0);
    }
  }
  cout << "Configuration Loaded"<<endl;
  while (fgets(buffer,255,fp) != NULL)
  {
    ++*buffer;
    cout << *buffer << endl;
    parseLine(buffer);
  }
//  delete buffer;
}
EsbConfig::EsbConfig(sockaddr_in serveraddr)
{}

EsbConfig::~EsbConfig()
{
}
/**
 * retriven der Konfigurationdaten
 */
char * EsbConfig::getConfig(char * key)
{
  cout << "Config:"<<key<<"\t"<<config_map[key]<<":"<<endl;
  return config_map[key];
}
/**
* @todo überarbeiten der Config Klasse
* Fehler bei leerzeilen in der Konfiguration
*/
void parseLine(char*line)
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
	cout << "Line:"<<line<<endl;
        char *key=new char[seperator];
        short max_copy=length-seperator-1;
        char *val=new char[max_copy];
        bzero(key,sizeof(key));
        bzero(val,sizeof(val));
        memcpy(key,line,seperator);
        memcpy(val,line+(seperator+1),max_copy);
        config_map[key]=val;
	cout << "Key:"<< key<<"\tVal:"<<val<<endl;
      }
    }
  }
}
  /*
    void start_config_server(){
        struct sockaddr_in server, client;
        int listenfd=socket(AF_INET,SOCK_STREAM,0);
        bzero(&server,sizeof(server));
        server.sin_family=AF_INET;
        server.sin_addr.s_addr=htonl(INADDR_ANY);
        server.sin_port=htons(atoi(EsbConfig::getConfig("configport")));
        if(bind(listenfd,(struct sockaddr*)&server, sizeof(server))<0){
      perror("Bind");
      close(listenfd);
      exit(1);
        }
        listen(listenfd,1024);
        for(;;){
      socklen_t clilen;
      int connfd=accept(listenfd,(struct sockaddr*)&client,&clilen);
  //          Data *data=new (struct Data);
  //    data->sock=connfd;
  //    new_client(data);
        }
        close(listenfd);
    }
  */