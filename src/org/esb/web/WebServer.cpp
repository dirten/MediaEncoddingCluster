#include "WebServer.h"
#include <iostream>
#include "Files.h"
#include "Stream.h"
#include "Page.h"
#include "Jobs.h"
#include "WTK.h"
using namespace std;
using namespace org::esb::web;

bool WebServer::_toStop=false;

void testWtk(struct shttpd_arg *arg){

//    TableColumn col(TableRow(Table()));
    Table table;
//    for(int a=0;a<10;a++){
        TableRow row;
	TableColumn col;
        col.setValue("test");
        row.addColumn(col);
        table.addRow(row);
//    }
    
//    Table tab(TableRow(TableColumn("")));
//    Element *el=(Element*)&col;
//    string t=col.toHtml();
    shttpd_printf(arg, table.toHtml().c_str());
    arg->flags |= SHTTPD_END_OF_OUTPUT;
}


WebServer::WebServer(int port){
	char c_port[10];
	sprintf(c_port,"%d",port);
	ctx = shttpd_init();
	shttpd_set_option(ctx, "ports", c_port);
	shttpd_set_option(ctx, "root", "src/org/esb/web");
	shttpd_set_option(ctx, "access_log", "access.log");
	shttpd_set_option(ctx, "error_log", "error.log");
//	shttpd_register_uri(ctx, "/", &Page::showPage, (void *) NULL);
	shttpd_register_uri(ctx, "/upload", &Files::upload_file, (void *) NULL);
	shttpd_register_uri(ctx, "/test", &testWtk, (void *) NULL);
	shttpd_register_uri(ctx, "/save_stream_details", &Stream::save_output_stream, (void *) NULL);
	shttpd_register_ssi_func(ctx, "showFiles", &Files::show_files, NULL);
	shttpd_register_ssi_func(ctx, "showFileDetails", &Files::show_details, NULL);
//	shttpd_handle_error(ctx, 404, &Page::showPage, NULL);
	while (!WebServer::_toStop)
		shttpd_poll(ctx, 1000);
}

void WebServer::run(){


}
void WebServer::stop(){
	WebServer::_toStop=true;

}

