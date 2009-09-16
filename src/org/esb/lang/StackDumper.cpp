#include "StackDumper.h"



StackDumper::StackDumper():
	exhandler(
		L".",
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

