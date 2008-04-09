#include <iostream>
#include <stdio.h>
#include "org/esb/httpd/shttpd.h"
//#include "org/esb/web/WebServer.h"
#include "org/esb/config/config.h"
#include "tnt/tntnet.h"
using namespace std;
//using namespace org::esb::web;
using namespace org::esb::config;


static void show_post(struct shttpd_arg *arg)
{
	const char	*s, *path = "uploaded.txt";
	struct state {
		size_t	cl;		/* Content-Length	*/
		size_t	nread;		/* Number of bytes read	*/
		FILE	*fp;
	} * mystate;

	/* If the connection was broken prematurely, cleanup */
	if (arg->flags & SHTTPD_CONNECTION_ERROR && arg->state) {
		(void) fclose(((struct state *) arg->state)->fp);
		free(arg->state);
	} else if ((s = shttpd_get_header(arg, "Content-Length")) == NULL) {
		shttpd_printf(arg, "HTTP/1.0 411 Length Required\n\n");
		arg->flags |= SHTTPD_END_OF_OUTPUT;
	} else if (arg->state == NULL) {
		/* New request. Allocate a state structure, and open a file */
		arg->state = mystate = new state();//calloc(1, sizeof(state));
		mystate->cl = strtoul(s, NULL, 10);
		mystate->fp = fopen(path, "wb+");
		shttpd_printf(arg, "HTTP/1.0 200 OK\n"
			"Content-Type: text/plain\n\n");
		cout <<"FileName"<< shttpd_get_header(arg, "filename")<<endl;
		cout.flush();
	} else {
		mystate = (struct state*)arg->state;

		/*
		 * Write the POST data to a file. We do not do any URL
		 * decoding here. File will contain form-urlencoded stuff.
		 */
		(void) fwrite(arg->in.buf, arg->in.len, 1, mystate->fp);
		mystate->nread += arg->in.len;

		/* Tell SHTTPD we have processed all data */
		arg->in.num_bytes = arg->in.len;
//		if(mystate->nread%1000000<1000)
//			shttpd_printf(arg, "Written %d bytes to %s\n\n", mystate->nread, path);
//		cout << "Readed:"<<mystate->nread<<endl;
		/* Data stream finished? Close the file, and free the state */
		if (mystate->nread >= mystate->cl) {
			shttpd_printf(arg, "Written %d bytes to %s",
			    mystate->nread, path);
			(void) fclose(mystate->fp);
			delete mystate;
//			free(mystate);
			arg->flags |= SHTTPD_END_OF_OUTPUT;
		}
	}
}


static void show_index(struct shttpd_arg *arg){
	cout << "Request"<<endl;

	shttpd_printf(arg, "%s",
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
		"<html><body><h1>Welcome to embedded example of SHTTPD");

	shttpd_printf(arg, "<li>Upload file example. "
	    "<form method=\"post\" enctype=\"multipart/form-data\" "
	    "action=\"/post\"><input type=\"file\" name=\"file\">"
	    "<input type=\"submit\"></form>");

		arg->flags |= SHTTPD_END_OF_OUTPUT;

}

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

    Config::init("./cluster.cfg");
    tnt::Tntnet app;
    app.listen("0.0.0.0",8008);
    app.mapUrl("^/$","mainpage").setPathInfo("/mainpage");
    app.mapUrl("^/(.*)$","$1");
    app.mapUrl("^/(.*).png$","$1");
    app.run();
}

