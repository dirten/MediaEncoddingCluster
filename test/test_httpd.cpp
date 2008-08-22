#include <iostream>
#include <stdio.h>
#include "org/esb/config/config.h"
//#include "tnt/tntnet.h"
//#include "cxxtools/log.h"
//#include "cxxtools/loginit.h"
using namespace std;
//using namespace org::esb::web;
using namespace org::esb::config;


//logdefine("hive");
int main(){
/*	int			data = 1234567;
	struct shttpd_ctx	*ctx;
	
	ctx = shttpd_init();
	shttpd_set_option(ctx, "ports", "8080");
	shttpd_register_uri(ctx, "/", &show_index, (void *) &data);
	shttpd_register_uri(ctx, "/post", &show_post, NULL);

	for (;;)
		shttpd_poll(ctx, 1000);
*/
/*
	Config::init("./cluster.cfg");

	WebServer(atoi(Config::getProperty("web.listener.port")));
	return 0;
	
*/
	
//	log_define("hive")
/*
    Config::init("./cluster.cfg");
    log_init("log.properties");
    log_error("bla fasel");
    tnt::Tntnet app;
    app.listen("0.0.0.0",8008);
    app.mapUrl("^/$","mainpage").setPathInfo("/mainpage");
    app.mapUrl("^/(.*)$","$1");
    app.mapUrl("^/(.*).png$","$1");
    app.run();
  */  
}

