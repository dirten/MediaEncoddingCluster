#include "StackDumper.h"



StackDumper::StackDumper():
	exhandler(
#ifndef WIN32
		".",
#elif WIN32
		L".",
#endif
		NULL,
		NULL,
		NULL,
#ifndef WIN32
		true
#elif WIN32
		google_breakpad::ExceptionHandler::HANDLER_ALL
#endif
		){

}
StackDumper::~StackDumper(){

}

