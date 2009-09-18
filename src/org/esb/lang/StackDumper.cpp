#include <string>

#include "StackDumper.h"



StackDumper::StackDumper(std::string dmp_path):
	exhandler(
#ifndef WIN32
		dmp_path,
#elif WIN32
		std::wstring(dmp_path.begin(), dmp_path.end()),
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

