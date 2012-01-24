/* 
 * File:   HTTPException.cpp
 * Author: HoelscJ
 * 
 * Created on 24. Januar 2012, 13:17
 */

#include "HTTPException.h"

HTTPException::HTTPException() {
}
HTTPException::HTTPException(const char* file, const int lineNumber,const std::string msg, ...):Exception(file, lineNumber,msg,vaargs){
  
}

HTTPException::~HTTPException()throw() {
}

