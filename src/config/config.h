/*
 * aviserv.cpp
 *
 * Copyright 2001-2005, EsbLab (www.esblab.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <sys/socket.h>
#include <netinet/in.h>
class EsbConfig{
    public:
	EsbConfig(sockaddr_in);
	EsbConfig(char*);
	EsbConfig(char*, bool);
	static char * getConfig( char * );
    private:
	~EsbConfig();
	
};
