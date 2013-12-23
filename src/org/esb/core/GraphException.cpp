/* 
 * File:   GraphException.cpp
 * Author: jhoelscher
 * 
 * Created on 12. Dezember 2011, 10:41
 */

#include "GraphException.h"
namespace org {
  namespace esb {
    namespace core {

      GraphException::GraphException(std::string msg,std::string el):org::esb::lang::Exception(msg),_msg(msg) {
        _element=el;
      }

      GraphException::GraphException( const char* file, const int lineNumber, const std::string msg):org::esb::lang::Exception(file, lineNumber, msg, NULL){

      }

      GraphException::~GraphException() throw() {
      }
      const char * GraphException::what()throw(){
        return (_msg+":"+_element).c_str();
      }
      std::string GraphException::getElementId(){
        return _element;
      }
    }
  }
}
