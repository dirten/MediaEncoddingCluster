#include "WebServer.h"
#include <iostream>
#include "Files.h"
#include "Jobs.h"
using namespace std;
using namespace org::esb::web;

bool WebServer::_toStop=false;


static void show_index(struct shttpd_arg *arg){
//	cout << "Request"<<endl;

	shttpd_printf(arg, "%s",
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
		"<html><body><h1>Welcome to embedded example of SHTTPD");

	shttpd_printf(arg, "<li>Upload file example. "
	    "<form method=\"post\" enctype=\"multipart/form-data\" "
	    "action=\"/post\"><input type=\"file\" name=\"file\">"
	    "<input type=\"submit\"></form>");

		arg->flags |= SHTTPD_END_OF_OUTPUT;

}
/*
static void show_files(struct shttpd_arg *arg){
//	cout << "Request"<<endl;

	shttpd_printf(arg, "%s",
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	char value[20];
	const char * query_string = shttpd_get_env(arg, "QUERY_STRING");
	shttpd_get_var("name", query_string, strlen(query_string), value, sizeof(value));
	shttpd_printf(arg, "show files");
	shttpd_printf(arg, value);
	shttpd_printf(arg, query_string);

	arg->flags |= SHTTPD_END_OF_OUTPUT;

}
*/
static void ssi_test(struct shttpd_arg *arg){
//	cout << "test_ssi:"<<arg->user_data<<endl;
}

WebServer::WebServer(int port){
//	struct shttpd_ctx	*ctx;
	char c_port[10];
	sprintf(c_port,"%d",port);
	ctx = shttpd_init();
	shttpd_set_option(ctx, "ports", c_port);
	shttpd_set_option(ctx, "root", "src/org/esb/web");
	shttpd_set_option(ctx, "access_log", "access.log");
	shttpd_set_option(ctx, "error_log", "error.log");
	shttpd_register_uri(ctx, "/", &show_index, (void *) NULL);
	shttpd_register_uri(ctx, "/files/*", &Files::show_files, (void *) NULL);
	shttpd_register_uri(ctx, "/jobs/*", &Jobs::show_jobs, (void *) NULL);
//	shttpd_register_uri(ctx, "/files/", &show_files, (void *) NULL);

	shttpd_register_ssi_func(ctx, "showFiles", &Files::show_files, NULL);
	shttpd_register_ssi_func(ctx, "showJobs", &Jobs::show_jobs, NULL);

//	shttpd_register_uri(ctx, "/post", &show_post, NULL);

	while (!WebServer::_toStop)
		shttpd_poll(ctx, 1000);


}

void WebServer::run(){


}
void WebServer::stop(){
	WebServer::_toStop=true;

}

